// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Jormungandr/InteractionSystem/JInteractableActorBase.h"
#include "JPickUpItem.generated.h"

class AJInventoryItem;

UCLASS()
class JORMUNGANDR_API AJPickUpItem : public AJInteractableActorBase
{
	GENERATED_BODY()

public:
	AJPickUpItem();

	virtual void InteractPressTriggered_Implementation() override;

private:
	UPROPERTY(EditInstanceOnly, Category="J|PickUpItem")
	TObjectPtr<AJInventoryItem> ItemToPickUp;
};
