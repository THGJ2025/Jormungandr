// Fill out your copyright notice in the Description page of Project Settings.


#include "JRitualBell.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AJRitualBell::AJRitualBell()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("NoCollision"));
	BoxComponent->SetNotifyRigidBodyCollision(true); // Simulation Generates Hit Events

	BellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell Mesh"));
	BellMesh->SetupAttachment(GetRootComponent());
	BellMesh->SetCollisionProfileName(FName("NoCollision"));
	BellMesh->SetGenerateOverlapEvents(false);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bAutoActivate = false;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	RotatingMovementComponent->SetUpdatedComponent(GetRootComponent());
	RotatingMovementComponent->RotationRate = FRotator(-1440.f, 0.f, 0.f);
	RotatingMovementComponent->bAutoActivate = false;
}

void AJRitualBell::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (BellState)
	{
	case EBellState::EBS_None:
		UE_LOG(LogTemp, Warning, TEXT("None"))
		break;
	case EBellState::EBS_Idle:
		UE_LOG(LogTemp, Warning, TEXT("Idle"))
		break;
	case EBellState::EBS_PostLaunch:
		UE_LOG(LogTemp, Warning, TEXT("PostLaunch"))
		PostLaunchTick();
		break;
	case EBellState::EBS_FlyingToTarget:
		UE_LOG(LogTemp, Warning, TEXT("FlyToTarget"))
		FlyToTargetTick();
		break;
	case EBellState::EBS_AfterImpact:
		UE_LOG(LogTemp, Warning, TEXT("AfterImpact"))
		break;
	case EBellState::EBS_Max:
		UE_LOG(LogTemp, Warning, TEXT("Max"))
		break;
	}
}

void AJRitualBell::PrimaryUse()
{
	Super::PrimaryUse();

	Launch();
}

void AJRitualBell::SecondaryUse()
{
	Super::SecondaryUse();

	Empower();
}

void AJRitualBell::Launch()
{
	BoxComponent->SetCollisionProfileName(FName("BlockAllDynamic"));
	FCollisionResponseContainer Responses;
	Responses.SetResponse(ECC_Pawn, ECR_Ignore);
	Responses.SetResponse(ECC_PhysicsBody, ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannels(Responses);

	FVector CrosshairsWorldPosition{};
	FVector CrosshairsWorldDirection{};
	const APlayerController* PlayerController{UGameplayStatics::GetPlayerController(this, 0)};
	const FVector2D ScreenCenter{UWidgetLayoutLibrary::GetViewportSize(this) / 2};
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenCenter, CrosshairsWorldPosition, CrosshairsWorldDirection);

	ProjectileMovementComponent->Velocity = Speed * CrosshairsWorldDirection;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(CrosshairsWorldPosition);
	SetActorRotation(GetOwner()->GetActorRotation());
	ProjectileMovementComponent->Activate();
	RotatingMovementComponent->Activate();

	InitialLocation = CrosshairsWorldPosition;

	BellState = EBellState::EBS_PostLaunch;
}

void AJRitualBell::Empower()
{
}

void AJRitualBell::Recall()
{
	ProjectileMovementComponent->Deactivate();
	RotatingMovementComponent->Deactivate();

	const FVector OwnerLocation{GetOwner()->GetTargetLocation()};
	const FQuat OwnerRotation{GetOwner()->GetActorRotation()};
	FlyToTarget(OwnerLocation, OwnerRotation);

	BoxComponent->SetCollisionProfileName(FName("NoCollision"));
}

void AJRitualBell::FlyToTarget(const FVector& TargetLocation, const FQuat& TargetRotation)
{
	BellState = EBellState::EBS_FlyingToTarget;

	InitialLocation = GetActorLocation();
	FlightInfo.TargetLocation = TargetLocation;
	FlightInfo.TargetRotation = TargetRotation;

	const double DistanceToTarget{FVector::Distance(FlightInfo.TargetLocation, InitialLocation)};

	FlightInfo.Duration = FMath::Min(DistanceToTarget / RecallMinSpeed, RecallMaxDuration);

	GetWorldTimerManager().SetTimer(FlightInfo.FlightTimer, this, &AJRitualBell::OnFlightTimerCompleted, FlightInfo.Duration, false);
}

void AJRitualBell::PostLaunchTick()
{
	const FVector CurrentLocation{GetActorLocation()};

	if (FVector::Distance(InitialLocation, CurrentLocation) >= MaxDistance)
	{
		Recall();
	}
}

void AJRitualBell::FlyToTargetTick()
{
	InterpToTargetLocation();
}

void AJRitualBell::InterpToTargetLocation()
{
	const float TimeElapsed{GetWorldTimerManager().GetTimerElapsed(FlightInfo.FlightTimer)};
	// The percentage of how close it is to the target location.
	const float Alpha{TimeElapsed / FlightInfo.Duration};

	const FVector NewLocation{FMath::Lerp(InitialLocation, FlightInfo.TargetLocation, Alpha)};
	UE_LOG(LogTemp, Warning, TEXT("InitialLocation: %s, TargetLocation: %s, Alpha: %f"), *InitialLocation.ToString(), *FlightInfo.TargetLocation.ToString(), Alpha)
	SetActorLocation(NewLocation);
}

void AJRitualBell::OnFlightTimerCompleted()
{
	BellState = EBellState::EBS_Idle;

	SetActorLocation(FlightInfo.TargetLocation);
	SetActorRotation(FlightInfo.TargetRotation);

	OnFlightCompletedDelegate.ExecuteIfBound();
}
