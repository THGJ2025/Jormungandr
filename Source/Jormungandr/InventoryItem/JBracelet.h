// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JBracelet.generated.h"

UCLASS()
class JORMUNGANDR_API AJBracelet : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJBracelet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category="J|Bracelet|Mesh")
	TObjectPtr<UStaticMeshComponent> BraceletMesh;
};
