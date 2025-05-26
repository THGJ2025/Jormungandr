// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JItemManagerComponent.generated.h"

class AJUsableItem;

UCLASS(ClassGroup=(Items), meta=(BlueprintSpawnableComponent))
class JORMUNGANDR_API UJItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJItemManagerComponent();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="J|Item Manager")
	void AddItem(AJUsableItem* NewItem);
	void NextItem();
	void PreviousItem();

	void PrimaryUseActiveItem();
	void SecondaryUseActiveItem();

private:
	UPROPERTY()
	TArray<AJUsableItem*> Items;

	UPROPERTY()
	TObjectPtr<AJUsableItem> ActiveItem;

	uint8 CurrentItemIndex{};
};
