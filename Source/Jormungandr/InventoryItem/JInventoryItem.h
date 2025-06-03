// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JInventoryItem.generated.h"

UCLASS()
class JORMUNGANDR_API AJInventoryItem : public AActor
{
	GENERATED_BODY()

public:
	AJInventoryItem();

	virtual void PrimaryUse();
	virtual void SecondaryUse();
};
