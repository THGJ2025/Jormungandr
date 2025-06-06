// Fill out your copyright notice in the Description page of Project Settings.


#include "JInventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "JBracelet.h"
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
	if (!Bracelet && ((Bracelet = Cast<AJBracelet>(NewItem))))
	{
		Bracelet->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("wrist_l_Socket"));
		return;
	}

	Items.Add(NewItem);

	if (Items.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem: first item - setting new active item"))
		SetActiveItem(0);
	}
	else
	{
		Items.Sort();
	}

	NewItem->SetInstigator(Cast<APawn>(GetOwner()));
}

void UJInventoryComponent::ChangeActiveItem(const FInputActionValue& Value)
{
	if (Items.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeActiveItem - can't change active item, because there are no items"))
		return;
	}
	if (Items.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeActiveItem - can't change active item, because there is only one item"))
		return;
	}

	const uint8 ChangeValue = Value.Get<float>();
	// Wraps index from 0 to size of array, works in both ways for -1 and 1
	ActiveItemIndex = (ActiveItemIndex + ChangeValue + Items.Num()) % Items.Num();

	UE_LOG(LogTemp, Warning, TEXT("ChangeValue: %d"), ChangeValue)
	SetActiveItem(ActiveItemIndex);
}

void UJInventoryComponent::SetFirstItemAsActiveItem()
{
	ActiveItemIndex = 0;
	SetActiveItem(ActiveItemIndex);
}

void UJInventoryComponent::SetSecondItemAsActiveItem()
{
	ActiveItemIndex = 1;
	SetActiveItem(ActiveItemIndex);
}

void UJInventoryComponent::SetThirdItemAsActiveItem()
{
	ActiveItemIndex = 2;
	SetActiveItem(ActiveItemIndex);
}

void UJInventoryComponent::UseFirstAbilityActiveItem()
{
	if (!ActiveItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseFirstAbilityActiveItem - can't use first ability, because no active item"))
		return;
	}
	if (ActiveItem->GetIsInUse())
	{
		UE_LOG(LogTemp, Warning, TEXT("UseFirstAbilityActiveItem - can't use first ability, because active item is in use."))
		return;
	}

	ActiveItem->UseFirstAbility();
}

void UJInventoryComponent::UseSecondAbilityActiveItem()
{
	if (!ActiveItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseSecondAbilityActiveItem - can't use second ability, because no active item"))
		return;
	}
	if (ActiveItem->GetIsInUse())
	{
		UE_LOG(LogTemp, Warning, TEXT("UseSecondAbilityActiveItem - can't use second ability, because active item is in use"))
		return;
	}

	ActiveItem->UseSecondAbility();
}

void UJInventoryComponent::SetActiveItem(const uint8 ItemIndex)
{
	if (!Items.IsValidIndex(ItemIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveItem: not valid index"))
		return;
	}
	if (ActiveItem == Items[ItemIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveItem: Can't change item it's the same item"))
		return;
	}

	if (ActiveItem)
	{
		if (ActiveItem->GetIsInUse())
		{
			UE_LOG(LogTemp, Warning, TEXT("SetActiveItem: can't change active item, because active item is in use"))
			return;
		}
		ActiveItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ActiveItem->SetActorLocation(FVector(0.f, 0.f, -1000.f));
		UE_LOG(LogTemp, Warning, TEXT("SetActiveItem: Detaching from actor and setting location: 0,0,-1000"))
	}

	ActiveItem = Items[ItemIndex];
	ActiveItem->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_Socket"));
	UE_LOG(LogTemp, Warning, TEXT("SetActiveItem: New active item, attach to hand_r_Socket"))
}

void UJInventoryComponent::SetupInput()
{
	UVALID_LOG_DEBUG(CharacterOwner)
	UVALID_LOG_DEBUG(InventoryMappingContext)
	UVALID_LOG_DEBUG(ChangeActiveItemAction)
	UVALID_LOG_DEBUG(SetFirstItemAsActiveItemAction)
	UVALID_LOG_DEBUG(SetSecondItemAsActiveItemAction)
	UVALID_LOG_DEBUG(SetThirdItemAsActiveItemAction)
	UVALID_LOG_DEBUG(FirstAbilityAction)
	UVALID_LOG_DEBUG(SecondAbilityAction)

	const APlayerController* PlayerController{CharacterOwner->GetController<APlayerController>()};

	UVALID_LOG_DEBUG(PlayerController)

	UEnhancedInputLocalPlayerSubsystem* Subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())};
	UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(PlayerController->InputComponent)};

	UVALID_LOG_DEBUG(Subsystem)
	UVALID_LOG_DEBUG(EnhancedInputComponent)

	Subsystem->AddMappingContext(InventoryMappingContext, 0);

	EnhancedInputComponent->BindAction(ChangeActiveItemAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::ChangeActiveItem);
	EnhancedInputComponent->BindAction(SetFirstItemAsActiveItemAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::SetFirstItemAsActiveItem);
	EnhancedInputComponent->BindAction(SetSecondItemAsActiveItemAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::SetSecondItemAsActiveItem);
	EnhancedInputComponent->BindAction(SetThirdItemAsActiveItemAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::SetThirdItemAsActiveItem);
	EnhancedInputComponent->BindAction(FirstAbilityAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::UseFirstAbilityActiveItem);
	EnhancedInputComponent->BindAction(SecondAbilityAction, ETriggerEvent::Triggered, this, &UJInventoryComponent::UseSecondAbilityActiveItem);
}
