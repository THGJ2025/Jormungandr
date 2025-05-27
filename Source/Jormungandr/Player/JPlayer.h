// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JPlayer.generated.h"

class UJInteractComponent;
class UInputAction;

UCLASS()
class JORMUNGANDR_API AJPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AJPlayer();

#pragma region Interact

public:
	FORCEINLINE UJInteractComponent* GetInteractComponent()
	{
		return InteractComponent;
	}

private:
	UPROPERTY(VisibleAnywhere, meta=(DisplayName="Interact Component"))
	TObjectPtr<UJInteractComponent> InteractComponent;

#pragma endregion
};
