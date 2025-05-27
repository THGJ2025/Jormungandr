// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JInteractInterface.generated.h"

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	EIT_Press UMETA(DisplayName="Press"),
	EIT_Hold UMETA(DisplayName="Hold")
};

UENUM(BlueprintType)
enum class EInteractCategory : uint8
{
	EIC_Use UMETA(DisplayName="Use"),
	EIC_Collect UMETA(DisplayName="Collect"),
	EIC_Open UMETA(DisplayName="Open"),
	EIC_Unlock UMETA(DisplayName="Unlock")
};

UINTERFACE()
class UJInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class IJInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="J|Interact")
	void DisplayInteractionUI(const bool bShowInteract);

	/**
	 * Called when an interaction press is initiated by an actor.
	 *
	 * @param OtherInstigator The actor initiating the interaction.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact|Press")
	void InteractPressStarted(AActor* OtherInstigator);
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact|Press")
	void InteractPressTriggered();
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact|Press")
	void InteractPressCompleted();

	/**
	 * Called when an interaction hold is initiated by an actor.
	 *
	 * @param OtherInstigator The actor initiating the interaction.
	 * @return The hold threshold required to trigger the interaction.
	 */
	UFUNCTION(BlueprintNativeEvent, meta=(ReturnDisplayName="HoldTimeThreshold"), Category="J|Interact|Hold")
	float InteractHoldStarted(AActor* OtherInstigator);
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact|Hold")
	void InteractHoldOngoing(const float ElapsedSeconds);
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact|Hold")
	void InteractHoldCanceled();
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact|Hold")
	void InteractHoldTriggered();
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact|Hold")
	void InteractHoldCompleted();

	/**
	 * Override this if you need an attachment point other than the root component.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="J|Interact")
	USceneComponent* GetWidgetAttachmentComponent();
	/**
	 * Meant only to be called.
	 * Do NOT override this, unless you know what are you doing.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="J|Interact")
	bool DoesUseCustomInteractArea();
	/**
	 * Meant only to be called.
	 * Do NOT override this, unless you know what are you doing.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="J|Interact")
	void SetCanInteract(const bool bNewCanInteract);
	/**
	 * Meant only to be called.
	 * Do NOT override this, unless you know what are you doing.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="J|Interact")
	bool GetCanInteract();
	/**
	 * Meant only to be called.
	 * Do NOT override this, unless you know what are you doing.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="J|Interact")
	EInteractType GetInteractType();
};
