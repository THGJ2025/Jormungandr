// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JUsableItem.h"
#include "JLyre.generated.h"

UCLASS()
class JORMUNGANDR_API AJLyre : public AJUsableItem
{
	GENERATED_BODY()

public:
	AJLyre();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
