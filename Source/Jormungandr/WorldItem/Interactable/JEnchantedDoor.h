// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JEnchantedDoor.generated.h"

UCLASS()
class JORMUNGANDR_API AJEnchantedDoor : public AActor
{
	GENERATED_BODY()

public:
	AJEnchantedDoor();

	bool GetIsOpen()
	{
		return bIsOpen;
	}

	UFUNCTION(BlueprintNativeEvent, Category="J|Enchanted Door")
	void Open();

private:
	UPROPERTY(EditAnywhere, Category="J|Enchanted Door")
	bool bIsOpen{};
};
