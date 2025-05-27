// Fill out your copyright notice in the Description page of Project Settings.


#include "JInteractComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "JInteractInterface.h"
#include "GameFramework/Character.h"
#include "Jormungandr/Utility/WeredMacros.h"


UJInteractComponent::UJInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UJInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<ACharacter>(GetOwner());

	SetupInput();

	QueryParams.AddIgnoredActor(GetOwner());
}

void UJInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformInteractionCheck();
}

void UJInteractComponent::SetupInput()
{
	UVALID_LOG_DEBUG(CharacterOwner)
	UVALID_LOG_DEBUG(InteractMappingContext)
	UVALID_LOG_DEBUG(InteractPressAction)
	UVALID_LOG_DEBUG(InteractHoldAction)

	const APlayerController* PlayerController{CharacterOwner->GetController<APlayerController>()};

	UVALID_LOG_DEBUG(PlayerController)

	UEnhancedInputLocalPlayerSubsystem* Subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())};
	UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(PlayerController->InputComponent)};

	UVALID_LOG_DEBUG(Subsystem)
	UVALID_LOG_DEBUG(EnhancedInputComponent)

	Subsystem->AddMappingContext(InteractMappingContext, 0);

	EnhancedInputComponent->BindAction(InteractPressAction, ETriggerEvent::Started, this, &UJInteractComponent::InteractPressStarted);
	EnhancedInputComponent->BindAction(InteractPressAction, ETriggerEvent::Triggered, this, &UJInteractComponent::InteractPressTriggered);
	EnhancedInputComponent->BindAction(InteractPressAction, ETriggerEvent::Completed, this, &UJInteractComponent::InteractPressCompleted);

	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Started, this, &UJInteractComponent::InteractHoldStarted);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Ongoing, this, &UJInteractComponent::InteractHoldOngoing);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Canceled, this, &UJInteractComponent::InteractHoldCanceled);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Triggered, this, &UJInteractComponent::InteractHoldTriggered);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Completed, this, &UJInteractComponent::InteractHoldCompleted);
}

void UJInteractComponent::InteractPressStarted()
{
	UVALID_LOG_DEBUG(CharacterOwner)

	if (InteractableActor && InteractableActor->Implements<UJInteractInterface>() && IJInteractInterface::Execute_GetInteractType(InteractableActor) == EInteractType::EIT_Press)
	{
		IJInteractInterface::Execute_InteractPressStarted(InteractableActor, CharacterOwner);
	}
}

void UJInteractComponent::InteractPressTriggered()
{
	if (InteractableActor && InteractableActor->Implements<UJInteractInterface>() && IJInteractInterface::Execute_GetInteractType(InteractableActor) == EInteractType::EIT_Press)
	{
		IJInteractInterface::Execute_InteractPressTriggered(InteractableActor);
	}
}

void UJInteractComponent::InteractPressCompleted()
{
	if (InteractableActor && InteractableActor->Implements<UJInteractInterface>() && IJInteractInterface::Execute_GetInteractType(InteractableActor) == EInteractType::EIT_Press)
	{
		IJInteractInterface::Execute_InteractPressCompleted(InteractableActor);
	}
}

void UJInteractComponent::InteractHoldStarted(const FInputActionInstance& Instance)
{
	UVALID_LOG_DEBUG(CharacterOwner)

	if (!InteractableActor || !InteractableActor->Implements<UJInteractInterface>() || IJInteractInterface::Execute_GetInteractType(InteractableActor) != EInteractType::EIT_Hold)
	{
		return;
	}

	for (UInputTrigger* Trigger : Instance.GetTriggers())
	{
		if (UInputTriggerHold* TriggerHold{Cast<UInputTriggerHold>(Trigger)})
		{
			TriggerHold->HoldTimeThreshold = IJInteractInterface::Execute_InteractHoldStarted(InteractableActor, CharacterOwner);
			break;
		}
	}

	bIsHolding = true;
}

void UJInteractComponent::InteractHoldOngoing(const FInputActionInstance& Instance)
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UJInteractInterface>() && IJInteractInterface::Execute_GetInteractType(InteractableActor) == EInteractType::EIT_Hold)
	{
		IJInteractInterface::Execute_InteractHoldOngoing(InteractableActor, Instance.GetElapsedTime());
	}
}

void UJInteractComponent::InteractHoldCanceled()
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UJInteractInterface>() && IJInteractInterface::Execute_GetInteractType(InteractableActor) == EInteractType::EIT_Hold)
	{
		bIsHolding = false;

		IJInteractInterface::Execute_InteractHoldCanceled(InteractableActor);
	}
}

void UJInteractComponent::InteractHoldTriggered()
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UJInteractInterface>() && IJInteractInterface::Execute_GetInteractType(InteractableActor) == EInteractType::EIT_Hold)
	{
		IJInteractInterface::Execute_InteractHoldTriggered(InteractableActor);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UJInteractComponent::InteractHoldCompleted()
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UJInteractInterface>() && IJInteractInterface::Execute_GetInteractType(InteractableActor) == EInteractType::EIT_Hold)
	{
		bIsHolding = false;

		IJInteractInterface::Execute_InteractHoldCompleted(InteractableActor);
	}
}

void UJInteractComponent::PerformInteractionCheck()
{
	if (!bCanCheckInteraction)
	{
		return;
	}

	UVALID_LOG_DEBUG(CharacterOwner)

	const APlayerController* PlayerController{CharacterOwner->GetController<APlayerController>()};

	UVALID_LOG_DEBUG(PlayerController)

	// Get camera location and direction
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Define the start and end points of the line trace
	const FVector Start{CameraLocation};
	const FVector End{Start + CameraRotation.Vector() * InteractionDistance};

	// Perform the line trace
	FHitResult HitResult;
	const bool bHit
	{
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			QueryParams)
	};

	/**
	 * Clear interaction if at least one of below conditions is met:
	 * - nothing was hit
	 * - HitResult is not an actor
	 * - HitActor doesn't implement interact interface
	 * - HitActor is not allowed to interact
	 */
	AActor* HitActor{HitResult.GetActor()};
	UPrimitiveComponent* HitComponent{HitResult.GetComponent()};
	if (!bHit || !HitActor || !HitActor->Implements<UJInteractInterface>() || !IJInteractInterface::Execute_GetCanInteract(HitActor))
	{
		ClearInteraction();
		return;
	}

	const bool bUsesCustomInteractArea{IJInteractInterface::Execute_DoesUseCustomInteractArea(HitActor)};
	// Check if a custom interaction area is used:
	// compare components in this case
	if (bUsesCustomInteractArea && HitComponent == InteractableHitComponent)
	{
		return;
	}
	// otherwise, compare actors
	if (!bUsesCustomInteractArea && HitActor == InteractableActor)
	{
		return;
	}

	// Hide UI from previously focused interactable object
	if (InteractableActor)
	{
		IJInteractInterface::Execute_DisplayInteractionUI(InteractableActor, false);
	}

	// Depending on custom area usage, set or clear interaction
	if (bUsesCustomInteractArea)
	{
		if (HitComponent->GetCollisionProfileName() == FName("InteractArea"))
		{
			SetInteraction(HitActor, HitComponent);
		}
		else
		{
			ClearInteraction();
		}
	}
	else
	{
		SetInteraction(HitActor, HitComponent);
	}
}

void UJInteractComponent::SetInteraction(AActor* InteractedActor, UPrimitiveComponent* InteractedComponent)
{
	IJInteractInterface::Execute_DisplayInteractionUI(InteractedActor, true);
	InteractableActor = InteractedActor;
	InteractableHitComponent = InteractedComponent;
}

void UJInteractComponent::ClearInteraction()
{
	if (bIsHolding)
	{
		InteractHoldCanceled();
	}

	if (InteractableActor)
	{
		IJInteractInterface::Execute_DisplayInteractionUI(InteractableActor, false);
		InteractableActor = nullptr;
		InteractableHitComponent = nullptr;
	}
}
