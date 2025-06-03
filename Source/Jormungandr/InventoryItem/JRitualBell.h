// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JRitualBell.generated.h"

UCLASS()
class JORMUNGANDR_API AJRitualBell : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJRitualBell();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
