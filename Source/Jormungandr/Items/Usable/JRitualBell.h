// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JUsableItem.h"
#include "GameFramework/Actor.h"
#include "JRitualBell.generated.h"

class UBoxComponent;
class URotatingMovementComponent;
class UProjectileMovementComponent;

UCLASS()
class JORMUNGANDR_API AJRitualBell : public AJUsableItem
{
	GENERATED_BODY()

public:
	AJRitualBell();

public:
	virtual void PrimaryUse() override;
	virtual void SecondaryUse() override;

private:
	void Launch();
	void Empower();

	UPROPERTY(VisibleAnywhere, meta=(DisplayName="Box Collider"))
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, meta=(DisplayName="Bell Mesh"))
	TObjectPtr<UStaticMeshComponent> BellMesh;

	UPROPERTY(VisibleAnywhere, meta=(DisplayName="Projectile Movement Component"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, meta=(DisplayName="Rotating Movement Component"))
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;

	UPROPERTY(EditAnywhere, Category="J|Launch")
	float Speed{500.f};
};
