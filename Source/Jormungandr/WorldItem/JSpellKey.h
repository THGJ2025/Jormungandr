// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSpellKey.generated.h"

UCLASS()
class JORMUNGANDR_API AJSpellKey : public AActor
{
	GENERATED_BODY()

public:
	AJSpellKey();

	/**
	 * Deactivates the spell key if it is currently active.
	 *
	 * @return returns if collect was success.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="J|Spell Key")
	bool Collect();

private:
	UPROPERTY(VisibleAnywhere, Category="J|Spell Key|Properties")
	bool bActive{true};
};
