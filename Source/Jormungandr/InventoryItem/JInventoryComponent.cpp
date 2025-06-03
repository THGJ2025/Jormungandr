// Fill out your copyright notice in the Description page of Project Settings.


#include "JInventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "JInventoryItem.h"
#include "GameFramework/Character.h"
#include "Jormungandr/Utility/WeredMacros.h"


class UEnhancedInputLocalPlayerSubsystem;

UJInventoryComponent::UJInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<ACharacter>(GetOwner());

	SetupInput();
}

void UJInventoryComponent::AddItem(AJInventoryItem* NewItem)
{
	Items.Add(NewItem);

	if (Items.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem: first item - setting new active item"))
		SetActiveItem(NewItem);
	}
}

void UJInventoryComponent::ChangeActiveItem(const FInputActionValue& Value)
{
	if (Items.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeActiveItem - no items"))
		return;
	}
	if (Items.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeActiveItem - one item"))
		return;
	}

	// If false change to previous item, if true change to next item
	const float ChangeValue{Value.Get<float>()};
	if (ChangeValue > 0.f)
	{
		ActiveItemIndex = (ActiveItemIndex + 1) % Items.Num();
	}
	else
	{
		ActiveItemIndex = (ActiveItemIndex + Items.Num() - 1) % Items.Num();
	}
	// try
	// ActiveItemIndex = (ActiveItemIndex + ChangeValue + Items.Num()) % Items.Num();

	UE_LOG(LogTemp, Warning, TEXT("ChangeValue: %f"), ChangeValue)
	SetActiveItem(Items[ActiveItemIndex]);
}

void UJInventoryComponent::PrimaryUseActiveItem()
{
	if (!ActiveItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("PrimaryUseActiveItem: no active item"))
		return;
	}

	ActiveItem->PrimaryUse();
}

void UJInventoryComponent::SecondaryUseActiveItem()
{
	if (!ActiveItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SecondaryUseActiveItem: no active item"))
		return;
	}

	ActiveItem->SecondaryUse();
}

void UJInventoryComponent::SetActiveItem(AJInventoryItem* NewActiveItem)
{
	if (ActiveItem == NewActiveItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveItem: Same item"))
		return;
	}

	if (ActiveItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detaching from actor and setting location: 0,0,-1000"))
		ActiveItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ActiveItem->SetActorLocation(FVector(0.f, 0.f, -1000.f));
	}

	UE_LOG(LogTemp, Warning, TEXT("new active item, attach to hand_r_Socket"))
	ActiveItem = NewActiveItem;
	ActiveItem->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_Socket"));
}

void UJInventoryComponent::SetupInput()
{
	UVALID_LOG_DEBUG(CharacterOwner)
	UVALID_LOG_DEBUG(InventoryMappingContext)
	UVALID_LOG_DEBUG(PrimaryUseAction)
	UVALID_LOG_DEBUG(SecondaryUseAction)
	UVALID_LOG_DEBUG(ChangeActiveItemAction)

	const APlayerController* PlayerController{CharacterOwner->GetController<APlayerController>()};

	UVALID_LOG_DEBUG(PlayerController)

	UEnhancedInputLocalPlayerSubsystem* Subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())};
	UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(PlayerController->InputComponent)};

	UVALID_LOG_DEBUG(Subsystem)
	UVALID_LOG_DEBUG(EnhancedInputComponent)

	Subsystem->AddMappingContext(InventoryMappingContext, 0);

	EnhancedInputComponent->BindAction(ChangeActiveItemAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::ChangeActiveItem);
	EnhancedInputComponent->BindAction(PrimaryUseAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::PrimaryUseActiveItem);
	EnhancedInputComponent->BindAction(SecondaryUseAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::SecondaryUseActiveItem);
}
