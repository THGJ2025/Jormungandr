// Fill out your copyright notice in the Description page of Project Settings.


#include "JInventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "JBracelet.h"
#include "JInventoryItem.h"
#include "JUsableItem.h"
#include "GameFramework/Character.h"
#include "Jormungandr/Utility/WeredMacros.h"

// TODO delete forward declaration
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
		Bracelet->SetOwner(GetOwner());
		Bracelet->SetInstigator(Cast<APawn>(GetOwner()));
		return;
	}

	Items.Add(Cast<AJUsableItem>(NewItem));

	if (Items.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: first item - setting new active item"), TEXT(__FUNCTION__))
		SetActiveItem(0);
	}
	else
	{
		Items.Sort();
	}

	NewItem->SetOwner(GetOwner());
	NewItem->SetInstigator(Cast<APawn>(GetOwner()));
}

void UJInventoryComponent::SetActiveItem(const uint8 ItemIndex)
{
	if (!Items.IsValidIndex(ItemIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: not valid index"), TEXT(__FUNCTION__))
		return;
	}
	if (ActiveItem == Items[ItemIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Can't change item it's the same item"), TEXT(__FUNCTION__))
		return;
	}

	if (ActiveItem)
	{
		if (ActiveItem->GetIsInUse())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: can't change active item, because active item is in use"), TEXT(__FUNCTION__))
			return;
		}
		ActiveItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ActiveItem->SetActorLocation(FVector(0.f, 0.f, -1000.f));
		UE_LOG(LogTemp, Warning, TEXT("%s: Detaching from actor and setting location: 0,0,-1000"), TEXT(__FUNCTION__))
	}

	ActiveItem = Items[ItemIndex];
	ActiveItem->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_Socket"));
	UE_LOG(LogTemp, Warning, TEXT("%s: New active item, attach to hand_r_Socket"), TEXT(__FUNCTION__))
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

void UJInventoryComponent::ChangeActiveItem(const FInputActionValue& Value)
{
	if (Items.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: can't change active item, because there are no items"), TEXT(__FUNCTION__))
		return;
	}
	if (Items.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: can't change active item, because there is only one item"), TEXT(__FUNCTION__))
		return;
	}

	const uint8 ChangeValue = Value.Get<float>();
	// Wraps index from 0 to size of array, works in both ways for -1 and 1
	// TODO scrolling down doesnt change item at first
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
		UE_LOG(LogTemp, Warning, TEXT("%s: can't use first ability, because no active item"), TEXT(__FUNCTION__))
		return;
	}
	if (ActiveItem->GetIsInUse())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: can't use first ability, because active item is in use"), TEXT(__FUNCTION__))
		return;
	}

	ActiveItem->UseFirstAbility();
}

void UJInventoryComponent::UseSecondAbilityActiveItem()
{
	if (!ActiveItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: can't use second ability, because no active item"), TEXT(__FUNCTION__))
		return;
	}
	if (ActiveItem->GetIsInUse())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: can't use second ability, because active item is in use"), TEXT(__FUNCTION__))
		return;
	}

	ActiveItem->UseSecondAbility();
}
