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
	// Simulation Generates Hit Events
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

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
	FVector CrosshairsWorldPosition{};
	FVector CrosshairsWorldDirection{};
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), UWidgetLayoutLibrary::GetViewportSize(this) / 2, CrosshairsWorldPosition, CrosshairsWorldDirection);

	ProjectileMovementComponent->Velocity = Speed * CrosshairsWorldDirection;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(CrosshairsWorldPosition);
	SetActorRotation(GetOwner()->GetActorRotation());
	ProjectileMovementComponent->Activate();
	RotatingMovementComponent->Activate();
}

void AJRitualBell::Empower()
{
}
