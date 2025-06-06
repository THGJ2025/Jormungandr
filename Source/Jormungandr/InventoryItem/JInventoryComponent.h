// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JInventoryComponent.generated.h"

class AJBracelet;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class AJInventoryItem;

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class JORMUNGANDR_API UJInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddItem(AJInventoryItem* NewItem);

	void ChangeActiveItem(const FInputActionValue& Value);
	void SetFirstItemAsActiveItem();
	void SetSecondItemAsActiveItem();
	void SetThirdItemAsActiveItem();
	void UseFirstAbilityActiveItem();
	void UseSecondAbilityActiveItem();

private:
	void SetActiveItem(const uint8 ItemIndex);

	UPROPERTY()
	TObjectPtr<AJInventoryItem> ActiveItem;
	UPROPERTY()
	TArray<TObjectPtr<AJInventoryItem>> Items;
	uint8 ActiveItemIndex{};

	UPROPERTY()
	TObjectPtr<AJBracelet> Bracelet;

	UPROPERTY()
	TObjectPtr<ACharacter> CharacterOwner;

#pragma region Input

private:
	void SetupInput();

	UPROPERTY(EditDefaultsOnly, Category="J|Inventory|Input")
	TObjectPtr<UInputMappingContext> InventoryMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory|Input")
	TObjectPtr<UInputAction> ChangeActiveItemAction;
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory|Input")
	TObjectPtr<UInputAction> SetFirstItemAsActiveItemAction;
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory|Input")
	TObjectPtr<UInputAction> SetSecondItemAsActiveItemAction;
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory|Input")
	TObjectPtr<UInputAction> SetThirdItemAsActiveItemAction;
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory|Input")
	TObjectPtr<UInputAction> FirstAbilityAction;
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory|Input")
	TObjectPtr<UInputAction> SecondAbilityAction;

#pragma endregion
};
