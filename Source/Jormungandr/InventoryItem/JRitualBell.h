// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInventoryItem.h"
#include "JRitualBell.generated.h"

class URotatingMovementComponent;
class UProjectileMovementComponent;

DECLARE_DELEGATE(FOnFlightCompletedSignature)
DECLARE_DELEGATE(FOnBellRecallCompletedSignature)

UCLASS()
class JORMUNGANDR_API AJRitualBell : public AJInventoryItem
{
	GENERATED_BODY()

public:
	AJRitualBell();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void PrimaryUse() override;

#pragma region ThrowAndRecall

public:
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent,
	               AActor* OtherActor,
	               UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse,
	               const FHitResult& Hit);
	/**
	 * Delegate used to notify the subscriber when it has arrived at its target.
	 */
	FOnFlightCompletedSignature OnFlightCompletedDelegate;

	FOnBellRecallCompletedSignature OnBellRecallCompletedDelegate;

private:
	void Launch(const FVector& Velocity);

	void RecallAxe();

	/**
	 * Initiates the Bell to fly to a target. Mostly recalling to the right hand, but can also have a different target
	 */
	UFUNCTION()
	void FlyToTarget(const FQuat& TargetRotation, const FVector& TargetLocation, float MinSpeed, float FlightMaxDuration);

	/**
	 * Function called in Tick if the Bell is in the PostLaunch state.
	 */
	void PostLaunchTick();

	void FlyToTargetTick();

	/**
	 * Interpolates the Bell's location to its target location.
	 */
	void InterpToTargetLocation();

	/**
	 * Called once the FlightInfo.FlightTimer has completed.
	 * Changes its state and notifies all subscribers of the OnFlightCompleted delegate.
	 */
	void OnFlightTimerCompleted();

	/**
	 * Called when the Bell has completed its flight to its target.
	 * In this case, it's when the Bell recall is complete.
	 * Before the function is complete, it notifies it's subscriber (the player) that the recall is complete.
	 */
	void OnBellFlightCompleted();

	enum class EBellState : uint8
	{
		EBS_None,
		/**
		 * State when the axe is idle. Used when a player is holding the Ritual Bell.
		 */
		EBS_Idle,
		/**
		 * State when Bell has hit something after being launched by the projectile movement component.
		 */
		EBS_IdleAfterImpact,
		/**
		 * State immediately after it has been launched by the projectile movement component.
		 */
		EBS_PostLaunch,
		/**
		 * State when it is flying to a specific target and is flying without assistance from the projectile movement component.
		 */
		EBS_FlyingToTarget,
		EBS_Max
	};
	/**
	 * Used to encapsulate all the data responsible for flying the Bell to its target.
	 */
	struct FFlightInfo
	{
		FQuat InitialRotation{};
		FQuat TargetRotation{};
		FVector TargetLocation{};
		/**
		 * Timer used to calculate the alpha value between the initial location and the target location.
		 */
		FTimerHandle FlightTimer{};
		float Duration{};
		float InterpToTargetRotationDuration{};
	};

	FFlightInfo FlightInfo{};

	EBellState BellState{EBellState::EBS_Idle};

	UPROPERTY(EditAnywhere, Category="J|Properties")
	float FlightSpeed{500.f};

	UPROPERTY(EditAnywhere, Category="J|Properties")
	float MaxDistance{200.f};

	UPROPERTY(EditAnywhere, Category="J|Properties")
	FRotator BellThrowRotationRate{-1440.f, 0.f, 0.f};

	UPROPERTY(EditAnywhere, Category="J|Properties")
	float BellRecallMinSpeed{1600.f};

	UPROPERTY(EditAnywhere, Category="J|Properties")
	float BellRecallMaxDuration{1.f};

	FVector InitialLocation{};

	UPROPERTY(VisibleAnywhere, Category="J|Mesh")
	TObjectPtr<UStaticMeshComponent> BellMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;

#pragma endregion
};
