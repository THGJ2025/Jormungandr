// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JInventoryItem.generated.h"

UCLASS(Abstract)
class JORMUNGANDR_API AJInventoryItem : public AActor
{
	GENERATED_BODY()

public:
	AJInventoryItem();

	virtual void UseFirstAbility() {}
	virtual void UseSecondAbility() {}

	virtual bool GetIsInUse() { return false; }

	FORCEINLINE bool operator<(const AJInventoryItem& Item) const
	{
		return this->IndexOrder < Item.IndexOrder;
	}

private:
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory Item")
	uint8 IndexOrder{};
};
