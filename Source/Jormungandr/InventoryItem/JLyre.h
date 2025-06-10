// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JUsableItem.h"
#include "JLyre.generated.h"

class AJEnchantedDoor;
class AJSpellKey;

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

public:
	virtual void UseFirstAbility() override;
	virtual void UseSecondAbility() override;

private:
	UPROPERTY(VisibleAnywhere, Category="J|Lyre")
	TArray<AJSpellKey*> SpellKeys;

	UPROPERTY(VisibleAnywhere, Category="J|Lyre")
	TObjectPtr<AJSpellKey> SelectedSpellKey;

	UPROPERTY(VisibleAnywhere, Category="J|Lyre")
	TObjectPtr<AJEnchantedDoor> SelectedEnchantedDoor;

	UPROPERTY(EditAnywhere, Category="J|Lyre|Properties")
	float InteractionDistance{200.f};

	UPROPERTY(VisibleAnywhere, Category="J|Lyre|Mesh")
	TObjectPtr<UStaticMeshComponent> LyreMeshComponent;
};
