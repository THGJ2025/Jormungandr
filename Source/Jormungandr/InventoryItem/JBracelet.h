// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JBracelet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeChangedSignature, float, Charges);

UCLASS()
class JORMUNGANDR_API AJBracelet : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJBracelet();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	enum class EBraceletState
	{
		EBS_None,
		EBS_SlowCharge,
		EBS_FastCharge,
		EBS_Max
	};

	UFUNCTION(BlueprintGetter)
	float GetMaxCharges() const
	{
		return MaxCharges;
	}

	UFUNCTION(BlueprintGetter)
	float GetCurrentCharges() const
	{
		return CurrentCharges;
	}

	void AddCharge(const float Amount);
	bool SubCharge(const float Amount);

private:
	UPROPERTY(BlueprintAssignable)
	FOnChargeChangedSignature OnChargeChangedDelegate;

	UPROPERTY(VisibleAnywhere, Category="J|Bracelet|Mesh")
	TObjectPtr<UStaticMeshComponent> BraceletMesh;

	EBraceletState BraceletState{EBraceletState::EBS_SlowCharge};

	UPROPERTY(EditAnywhere, BlueprintGetter=GetMaxCharges, Category="J|Bracelet|Properties")
	float MaxCharges{5.f};
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentCharges, Category="J|Bracelet|Properties")
	float CurrentCharges{};
	UPROPERTY(EditAnywhere, Category="J|Bracelet|Properties")
	float SlowChargingAmount{0.1f};
	UPROPERTY(EditAnywhere, Category="J|Bracelet|Properties")
	float FastChargingAmount{1.f};
};
