// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JLyre.generated.h"

UCLASS()
class JORMUNGANDR_API AJLyre : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJLyre();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
