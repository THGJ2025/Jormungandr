// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInteractInterface.h"
#include "GameFramework/Actor.h"
#include "JInteractableActorBase.generated.h"

class UJInteractableComponent;

UCLASS()
class JORMUNGANDR_API AJInteractableActorBase : public AActor, public IJInteractInterface
{
	GENERATED_BODY()

public:
	AJInteractableActorBase();

	virtual void DisplayInteractionUI_Implementation(const bool bShowInteract) override;

	virtual void InteractPressStarted_Implementation(AActor* OtherInstigator) override;
	virtual void InteractPressTriggered_Implementation() override;
	virtual void InteractPressCompleted_Implementation() override;

	virtual float InteractHoldStarted_Implementation(AActor* OtherInstigator) override;
	virtual void InteractHoldOngoing_Implementation(const float ElapsedSeconds) override;
	virtual void InteractHoldTriggered_Implementation() override;
	virtual void InteractHoldCanceled_Implementation() override;
	virtual void InteractHoldCompleted_Implementation() override;

	virtual bool DoesUseCustomInteractArea_Implementation() override;
	virtual void SetCanInteract_Implementation(const bool bNewCanInteract) override;
	virtual bool GetCanInteract_Implementation() override;
	virtual EInteractType GetInteractType_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UJInteractableComponent> InteractableComponent;
};
