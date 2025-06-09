// Fill out your copyright notice in the Description page of Project Settings.


#include "JLyre.h"
#include "Jormungandr/Utility/WeredMacros.h"
#include "Jormungandr/WorldItem/JSpellKey.h"
#include "Jormungandr/WorldItem/Interactable/JEnchantedDoor.h"


AJLyre::AJLyre()
{
	PrimaryActorTick.bCanEverTick = true;

	LyreMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lyre Mesh Component"));
	SetRootComponent(LyreMeshComponent);
	LyreMeshComponent->SetCollisionProfileName(FName("NoCollision"));
}

void AJLyre::BeginPlay()
{
	Super::BeginPlay();
}

void AJLyre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO move it to BeginPlay, when inventory component is refactored
	APawn* PawnOwner{GetInstigator()};

	UVALID_LOG_DEBUG(PawnOwner)

	// TODO move it to BeginPlay, when inventory component is refactored
	const APlayerController* PlayerController{PawnOwner->GetController<APlayerController>()};

	UVALID_LOG_DEBUG(PlayerController)

	// Get camera location and direction
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Define the start and end points of the line trace
	const FVector Start{CameraLocation};
	const FVector End{Start + CameraRotation.Vector() * InteractionDistance};

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PawnOwner);

	// Perform the line trace
	FHitResult HitResult;
	const bool bHit
	{
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			QueryParams
		)
	};

	AActor* HitActor{HitResult.GetActor()};
	UPrimitiveComponent* HitComponent{HitResult.GetComponent()};

	if (!bHit || !HitActor)
	{
		return;
	}

	if (AJSpellKey* SpellKey{Cast<AJSpellKey>(HitActor)})
	{
		SelectedSpellKey = SpellKey;
	}
	else
	{
		SelectedSpellKey = nullptr;
	}

	if (AJEnchantedDoor* EnchantedDoor{Cast<AJEnchantedDoor>(HitActor)})
	{
		SelectedEnchantedDoor = EnchantedDoor;
	}
	else
	{
		SelectedEnchantedDoor = nullptr;
	}
}

void AJLyre::UseFirstAbility()
{
	Super::UseFirstAbility();

	if (SelectedSpellKey)
	{
		if (SelectedSpellKey->Collect())
		{
			SpellKeys.Add(SelectedSpellKey);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Spell Key is not active"), TEXT(__FUNCTION__))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: No Spell Key to collect"), TEXT(__FUNCTION__))
	}

	if (SelectedEnchantedDoor)
	{
		if (!SpellKeys.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Open state of Enchanted Door: %s"), TEXT(__FUNCTION__), SelectedEnchantedDoor->GetIsOpen() ? TEXT("Closed") : TEXT("Opened"))
			if (!SelectedEnchantedDoor->GetIsOpen())
			{
				SelectedEnchantedDoor->Open();
				SpellKeys.Pop();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s: Enchanted Door is already opened"), TEXT(__FUNCTION__))
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: No Spell Key to open door"), TEXT(__FUNCTION__))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: No Enchanted Door to open"), TEXT(__FUNCTION__))
	}
}

void AJLyre::UseSecondAbility()
{
	Super::UseSecondAbility();
}
