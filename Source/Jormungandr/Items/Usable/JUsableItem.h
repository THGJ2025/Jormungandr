// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JUsableItem.generated.h"

UCLASS()
class JORMUNGANDR_API AJUsableItem : public AActor
{
	GENERATED_BODY()

public:
	AJUsableItem();

public:
	virtual void PrimaryUse();
	virtual void SecondaryUse();
};
