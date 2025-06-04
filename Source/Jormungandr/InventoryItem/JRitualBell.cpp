// Fill out your copyright notice in the Description page of Project Settings.


#include "JRitualBell.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AJRitualBell::AJRitualBell()
{
	PrimaryActorTick.bCanEverTick = true;

	BellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell Mesh"));
	SetRootComponent(BellMesh);

	BellMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BellMesh->SetGenerateOverlapEvents(false);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	RotatingMovementComponent->SetUpdatedComponent(GetRootComponent());
	RotatingMovementComponent->RotationRate = FRotator(-1440.f, 0.f, 0.f);
	RotatingMovementComponent->bAutoActivate = false;
}

void AJRitualBell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJRitualBell::PrimaryUse()
{
	Super::PrimaryUse();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(GetInstigator()->GetControlRotation());

	FVector2D ViewportSize{};
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter{ViewportSize / 2};

	FVector ViewportCenterPosition{};
	FVector ViewportCenterDirection{};
	UGameplayStatics::DeprojectScreenToWorld(Cast<APlayerController>(GetInstigator()->GetController()), ViewportCenter, ViewportCenterPosition, ViewportCenterDirection);

	SetActorLocation(ViewportCenterPosition);

	Launch(ViewportCenterDirection * FlightSpeed);
}

void AJRitualBell::Launch(const FVector& Velocity)
{
	UE_LOG(LogTemp, Warning, TEXT("Launch Velocity: %s"), *Velocity.ToString())
	ProjectileMovementComponent->Velocity = Velocity;
	ProjectileMovementComponent->Activate();

	RotatingMovementComponent->Activate();
}
