// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JPlayer.generated.h"

class UJInventoryComponent;
class UJInteractComponent;
class UInputAction;

UCLASS()
class JORMUNGANDR_API AJPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AJPlayer();

#pragma region Interact

private:
	UPROPERTY(VisibleAnywhere, meta=(DisplayName="Interact Component"))
	TObjectPtr<UJInteractComponent> InteractComponent;

#pragma endregion

#pragma region Inventory

public:
	UFUNCTION(BlueprintGetter)
	UJInventoryComponent* GetInventoryComponent()
	{
		return InventoryComponent;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetInventoryComponent, meta=(DisplayName="Inventory Component"), Category="J|Inventory")
	TObjectPtr<UJInventoryComponent> InventoryComponent;

#pragma endregion
};
