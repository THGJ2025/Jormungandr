// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JBanishingCircleFragment.generated.h"

UCLASS()
class JORMUNGANDR_API AJBanishingCircleFragment : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJBanishingCircleFragment();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
