// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JInteractComponent.generated.h"

struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;

/**
 * Component that enables character to interact with object which has InteractInterface
 */
UCLASS(ClassGroup=(Interact), meta=(BlueprintSpawnableComponent), DisplayName="Interact Component")
class JORMUNGANDR_API UJInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJInteractComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**
	 * Stops performing interaction check but InteractableActor stay set
	 */
	FORCEINLINE void SetCanCheckInteraction(const bool bValue)
	{
		bCanCheckInteraction = bValue;
	}

private:
	void PerformInteractionCheck();
	void SetInteraction(AActor* InteractedActor, UPrimitiveComponent* InteractedComponent);
	void ClearInteraction();

	/**
	 * Reference to the actor currently being interacted with.
	 */
	UPROPERTY(VisibleAnywhere, Category="J|Interact")
	TObjectPtr<AActor> InteractableActor;
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> InteractableHitComponent;

	/**
	 * The maximum distance at which interaction with an object can occur.
	 */
	UPROPERTY(EditAnywhere, Category="J|Interact")
	float InteractionDistance{200.f};
	/**
	 * Determines if interaction checks can be performed.
	 */
	UPROPERTY(VisibleAnywhere, Category="J|Interact")
	bool bCanCheckInteraction{true};

	FCollisionQueryParams QueryParams;

	UPROPERTY()
	TObjectPtr<ACharacter> CharacterOwner;

#pragma region Input

private:
	void SetupInput();

	void InteractPressStarted();
	void InteractPressTriggered();
	void InteractPressCompleted();

	void InteractHoldStarted(const FInputActionInstance& Instance);
	void InteractHoldOngoing(const FInputActionInstance& Instance);
	void InteractHoldCanceled();
	void InteractHoldTriggered();
	void InteractHoldCompleted();

	UPROPERTY(EditDefaultsOnly, Category="J|Interact|Input")
	TObjectPtr<UInputMappingContext> InteractMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="J|Interact|Input")
	TObjectPtr<UInputAction> InteractPressAction;
	UPROPERTY(EditDefaultsOnly, Category="J|Interact|Input")
	TObjectPtr<UInputAction> InteractHoldAction;

	bool bIsHolding{false};

#pragma endregion
};
