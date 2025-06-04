// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JRitualBell.generated.h"

class URotatingMovementComponent;
class UProjectileMovementComponent;

UCLASS()
class JORMUNGANDR_API AJRitualBell : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJRitualBell();

	virtual void Tick(float DeltaTime) override;

	virtual void PrimaryUse() override;

#pragma region ThrowAndRecall

private:
	void Launch(const FVector& Velocity);

	UPROPERTY(EditAnywhere, Category="J|Properties")
	float FlightSpeed{500.f};

	UPROPERTY(VisibleAnywhere, Category="J|Mesh")
	TObjectPtr<UStaticMeshComponent> BellMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;

#pragma endregion
};
