// Fill out your copyright notice in the Description page of Project Settings.


#include "JRitualBell.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AJRitualBell::AJRitualBell()
{
	PrimaryActorTick.bCanEverTick = true;

	BellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell Mesh"));
	SetRootComponent(BellMesh);
	BellMesh->SetCollisionProfileName(FName("NoCollision"));
	BellMesh->SetGenerateOverlapEvents(false);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	RotatingMovementComponent->SetUpdatedComponent(GetRootComponent());
	RotatingMovementComponent->bAutoActivate = false;

	BellMesh->OnComponentHit.AddDynamic(this, &AJRitualBell::OnMeshHit);
}

void AJRitualBell::BeginPlay()
{
	Super::BeginPlay();
}

void AJRitualBell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (BellState)
	{
	case EBellState::EBS_None:
		break;
	case EBellState::EBS_Idle:
		break;
	case EBellState::EBS_IdleAfterImpact:
		break;
	case EBellState::EBS_PostLaunch:
		PostLaunchTick();
		break;
	case EBellState::EBS_FlyingToTarget:
		FlyToTargetTick();
		break;
	case EBellState::EBS_Max:
		break;
	}
}

void AJRitualBell::UseFirstAbility()
{
	Super::UseFirstAbility();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(GetInstigator()->GetControlRotation());

	FVector2D ViewportSize{};
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter{ViewportSize / 2};

	FVector ViewportCenterPosition{};
	FVector ViewportCenterDirection{};
	UGameplayStatics::DeprojectScreenToWorld(Cast<APlayerController>(GetInstigator()->GetController()),
	                                         ViewportCenter,
	                                         ViewportCenterPosition,
	                                         ViewportCenterDirection);

	InitialLocation = ViewportCenterPosition;
	SetActorLocation(ViewportCenterPosition);

	RotatingMovementComponent->RotationRate = BellThrowRotationRate;

	Launch(ViewportCenterDirection * FlightSpeed);
}

bool AJRitualBell::GetIsInUse()
{
	switch (BellState)
	{
	case EBellState::EBS_None:
	case EBellState::EBS_IdleAfterImpact:
	case EBellState::EBS_PostLaunch:
	case EBellState::EBS_FlyingToTarget:
	case EBellState::EBS_Max:
		return true;
	case EBellState::EBS_Idle:
		return false;
	}
	return false;
}

void AJRitualBell::OnMeshHit(UPrimitiveComponent* HitComponent,
                             AActor* OtherActor,
                             UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse,
                             const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Bell hit: %s"), *OtherActor->GetName())
	ProjectileMovementComponent->Deactivate();
	RotatingMovementComponent->Deactivate();
}

void AJRitualBell::Launch(const FVector& Velocity)
{
	BellMesh->SetCollisionProfileName(FName("IgnoreOnlyPawn"));
	ProjectileMovementComponent->Velocity = Velocity;
	ProjectileMovementComponent->Activate();

	RotatingMovementComponent->Activate();

	BellState = EBellState::EBS_PostLaunch;
}

void AJRitualBell::RecallBell()
{
	const FVector PlayerRightHandLocation{Cast<ACharacter>(GetInstigator())->GetMesh()->GetSocketLocation(FName("hand_r_Socket"))};

	FlyToTarget(GetInstigator()->GetActorQuat(), PlayerRightHandLocation, BellRecallMinSpeed, BellRecallMaxDuration);
}

void AJRitualBell::FlyToTarget(const FQuat& TargetRotation, const FVector& TargetLocation, float MinSpeed, float FlightMaxDuration)
{
	BellState = EBellState::EBS_FlyingToTarget;

	InitialLocation = GetActorLocation();

	FlightInfo.TargetLocation = TargetLocation;
	FlightInfo.TargetRotation = TargetRotation;

	const double DistanceToTarget{FVector::Dist(FlightInfo.TargetLocation, InitialLocation)};

	const FQuat FlightRotation{FlightInfo.TargetRotation * FQuat(FVector::RightVector, FMath::DegreesToRadians(90.f))};
	SetActorRotation(FlightRotation);

	/**
	 * If the time calculated based on MinSpeed is less than FlightMaxDuration, it will be the chosen duration of the flight.
	 */
	FlightInfo.Duration = FMath::Min(DistanceToTarget / MinSpeed, FlightMaxDuration);

	GetWorldTimerManager().SetTimer(FlightInfo.FlightTimer, this, &AJRitualBell::OnFlightTimerCompleted, FlightInfo.Duration, false);
}

void AJRitualBell::PostLaunchTick()
{
	if (FVector::Dist(InitialLocation, GetActorLocation()) >= MaxDistance)
	{
		ProjectileMovementComponent->Deactivate();
		RotatingMovementComponent->Deactivate();

		RecallBell();
	}
}

void AJRitualBell::FlyToTargetTick()
{
	InterpToTargetLocation();
}

void AJRitualBell::InterpToTargetLocation()
{
	const float TimeElapsed{GetWorldTimerManager().GetTimerElapsed(FlightInfo.FlightTimer)};

	/**
	 * The percentage of how close it is to the tagret location.
	 */
	const double Alpha{TimeElapsed / FlightInfo.Duration};

	const FVector NewLocation{FMath::Lerp(InitialLocation, FlightInfo.TargetLocation, Alpha)};
	UE_LOG(LogTemp, Warning, TEXT("NewLocation: %s"), *NewLocation.ToString())
	SetActorLocation(NewLocation);
}

void AJRitualBell::OnFlightTimerCompleted()
{
	BellMesh->SetCollisionProfileName(FName("NoCollision"));

	SetActorRotation(FlightInfo.TargetRotation);
	SetActorLocation(FlightInfo.TargetLocation);

	AttachToComponent(Cast<ACharacter>(GetInstigator())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_Socket"));
	BellState = EBellState::EBS_Idle;

	OnFlightCompletedDelegate.ExecuteIfBound();
}
