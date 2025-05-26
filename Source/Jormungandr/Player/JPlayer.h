// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JPlayer.generated.h"

class UInputAction;
class UJItemManagerComponent;

UCLASS()
class JORMUNGANDR_API AJPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AJPlayer();

#pragma region Items

public:
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UJItemManagerComponent* GetItemManager()
	{
		return ItemManagerComponent;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetItemManager, meta=(DisplayName="Item Manager"))
	TObjectPtr<UJItemManagerComponent> ItemManagerComponent;

#pragma endregion

#pragma region Input

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void PrimaryUseActiveItem();
	void SecondaryUseActiveItem();

private:
	UPROPERTY(EditDefaultsOnly, Category="J|Input")
	TObjectPtr<UInputAction> PrimaryUseActiveItemAction;
	UPROPERTY(EditDefaultsOnly, Category="J|Input")
	TObjectPtr<UInputAction> SecondaryUseActiveItemAction;

#pragma endregion
};
