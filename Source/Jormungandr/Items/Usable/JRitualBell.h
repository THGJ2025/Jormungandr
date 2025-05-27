// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JUsableItem.h"
#include "GameFramework/Actor.h"
#include "JRitualBell.generated.h"

class UBoxComponent;
class URotatingMovementComponent;
class UProjectileMovementComponent;

DECLARE_DELEGATE(FOnFlightCompletedSignature)

UCLASS()
class JORMUNGANDR_API AJRitualBell : public AJUsableItem
{
	GENERATED_BODY()

public:
	AJRitualBell();

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	enum class EBellState : uint8
	{
		EBS_None,
		/**
		 * State when the bell is idle. Used when the player is holding the bell.
		 */
		EBS_Idle,
		/**
		 * State immediately after it has been launched be the projectile movement component.
		 */
		EBS_PostLaunch,
		/**
		 * State when it is flying to a specific target and is flying without assistance from the projectile movement component. Used when recalling to the player.
		 */
		EBS_FlyingToTarget,
		/**
		 * State after the bell hit something after being launched by the projectile movement component.
		 */
		EBS_AfterImpact,
		EBS_Max
	};

	virtual void PrimaryUse() override;
	virtual void SecondaryUse() override;

public:
	FOnFlightCompletedSignature OnFlightCompletedDelegate;

private:
	void Launch();
	void Empower();

	void Recall();
	void FlyToTarget(const FVector& TargetLocation, const FQuat& TargetRotation);

	void PostLaunchTick();
	void FlyToTargetTick();

	void InterpToTargetLocation();

	void OnFlightTimerCompleted();

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

	UPROPERTY(VisibleAnywhere, Category="J|Launch")
	FVector InitialLocation{};

	UPROPERTY(EditAnywhere, Category="J|Launch")
	float MaxDistance{500.f};

	UPROPERTY(EditAnywhere, Category="J|Launch")
	float RecallMaxDuration{1.f};

	UPROPERTY(EditAnywhere, Category="J|Launch")
	float RecallMinSpeed{1600.f};

	EBellState BellState{EBellState::EBS_Idle};

	/**
	 * Used to encapsulate all the data responsible for flying the axe to it's target
	 */
	struct FFlightInfo
	{
		FQuat TargetRotation{};
		FVector TargetLocation{};

		/**
		 * Timer used to calculate the alpha value between the initial location and the target location.
		 */
		FTimerHandle FlightTimer{};

		float Duration{};
	};

	FFlightInfo FlightInfo;

	// TODO max distance, recall
};
