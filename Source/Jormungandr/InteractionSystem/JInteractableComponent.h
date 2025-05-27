// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInteractInterface.h"
#include "Components/ActorComponent.h"
#include "JInteractableComponent.generated.h"

class UWidgetComponent;
class UJInteractIconWidget;
enum class EInteractType : uint8;
enum class EInteractCategory : uint8;

/**
 * This component provides support for implementing interactable objects and handling player interaction logic.
 * Only for InteractableActorBase, InteractablePawnBase (not added yet), InteractableCharacterBase (not added yet).
 */
UCLASS(NotBlueprintable, NotBlueprintType, DisplayName="Interactable Component")
class JORMUNGANDR_API UJInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJInteractableComponent();

	virtual void BeginPlay() override;

	void SetInteractInstigator(AActor* Instigator)
	{
		InteractInstigator = Instigator;
	}

	AActor* GetInteractInstigator() const
	{
		return InteractInstigator;
	}

	UFUNCTION(BlueprintCallable, Category="J|Interact")
	void AddOutlineMeshComponent(UMeshComponent* MeshComponent);
	UFUNCTION(BlueprintCallable, Category="J|Interact")
	void SetOutlineMeshComponents(const TArray<UMeshComponent*>& NewOutlineMeshComponents);

	// Interact interface functions begin

	void DisplayInteractionUI(const bool bShowInteract);

	void InteractPressStarted(AActor* OtherInstigator);
	void InteractPressTriggered();
	void InteractPressCompleted();

	float InteractHoldStarted(AActor* OtherInstigator);
	void InteractHoldOngoing(const float ElapsedSeconds);
	void InteractHoldTriggered();
	void InteractHoldCanceled();
	void InteractHoldCompleted();

	bool DoesUseCustomInteractArea() const;
	void SetCanInteract(const bool bNewCanInteract);
	bool GetCanInteract() const;
	EInteractType GetInteractType() const;

	// Interact interface functions end

	void SetInteractCategory(const EInteractCategory InInteractCategory);

	UPROPERTY(EditAnywhere, Category="J|Interact")
	EInteractCategory InteractCategory{};
	UPROPERTY(EditAnywhere, Category="J|Interact")
	EInteractType InteractType{};

	UPROPERTY(EditAnywhere, Category="J|Interact")
	FVector2D IconSize{50.f, 50.f};

	UPROPERTY(EditAnywhere, meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1", EditCondition="InteractType == EInteractType::EIT_Hold"), Category="J|Interact")
	float MinimalIconOpacity{0.3f};
	/**
	 * Minimal opacity of foreground circle.
	 */
	UPROPERTY(EditAnywhere, meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1", EditCondition="InteractType == EInteractType::EIT_Hold"), Category="J|Interact")
	float MinimalProgressCircleOpacity{};
	UPROPERTY(EditAnywhere, meta=(EditCondition="InteractType == EInteractType::EIT_Hold"), Category="J|Interact")
	FVector2D ProgressCircleSize{100.f, 100.f};

	/**
	 * Required time duration (in seconds) to trigger and complete a hold interaction (it's one shot hold trigger).
	 */
	UPROPERTY(EditAnywhere, meta=(UIMin="0", ClampMin="0", EditCondition="InteractType == EInteractType::EIT_Hold"), Category="J|Interact")
	float HoldTimeThreshold{};

	UPROPERTY(EditAnywhere, Category="J|Interact")
	bool bCanInteract{true};
	/**
	 * Determines whether a custom interaction area is used for this interactable object.
	 * If true, add collision with preset "InteractArea".
	 */
	UPROPERTY(EditAnywhere, Category="J|Interact")
	bool bUseCustomInteractArea{};

protected:
	virtual void OnRegister() override;

private:
	void InitializeInteractWidget();
	void UpdateWidgetAttachment() const;

	/**
	 * Updates the outline rendering state for associated mesh components.
	 *
	 * @param bHighlight Determines whether the outline should be enabled or disabled.
	 */
	void UpdateOutline(const bool bHighlight);

	/**
	 * Pointer to the actor initiating the interaction, updated during interaction events.
	 * Used to track and manage the instigator of a specific interaction session.
	 */
	UPROPERTY(VisibleInstanceOnly, Category="J|Interact")
	TObjectPtr<AActor> InteractInstigator;

	UPROPERTY(VisibleAnywhere, Category="J|Interact")
	TArray<TObjectPtr<UMeshComponent>> OutlineMeshComps;

	/**
	 * Defines the widget class used to represent the interaction UI.
	 * By default widget comp is attached to root.
	 * You can override GetWidgetAttachment function from interact interface to specify new attachment.
	 */
	UPROPERTY(EditDefaultsOnly, meta=(DisplayPriority=0), Category="J|Interact")
	TSubclassOf<UJInteractIconWidget> InteractWidgetClass;

	UPROPERTY()
	TObjectPtr<UJInteractIconWidget> InteractWidget;
	UPROPERTY()
	TObjectPtr<UWidgetComponent> InteractWidgetComp;
};
