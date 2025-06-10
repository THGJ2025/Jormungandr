// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JUsableItem.generated.h"

UCLASS()
class JORMUNGANDR_API AJUsableItem : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJUsableItem();

	virtual void UseFirstAbility() {}
	virtual void UseSecondAbility() {}

	virtual bool GetIsInUse() { return false; }

	FORCEINLINE bool operator<(const AJUsableItem& Item) const
	{
		return this->IndexOrder < Item.IndexOrder;
	}

private:
	UPROPERTY(EditDefaultsOnly, Category="J|Inventory Item")
	uint8 IndexOrder{};
};
