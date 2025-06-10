// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JUsableItem.h"
#include "JRitualBell.generated.h"

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
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void UseFirstAbility() override;
	virtual void UseSecondAbility() override;

	virtual bool GetIsInUse() override;

private:
	bool bEmpowered{false};

#pragma region ThrowAndRecall

public:
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent,
	               AActor* OtherActor,
	               UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse,
	               const FHitResult& Hit);


	UFUNCTION(BlueprintImplementableEvent)
	void OnHitBreakableObject();

	/**
	 * Delegate used to notify the subscriber when it has arrived at its target.
	 */
	FOnFlightCompletedSignature OnFlightCompletedDelegate;

private:
	void Launch(const FVector& Velocity);

	void RecallBell();

	/**
	 * Initiates the Bell to fly to a target. Mostly recalling to the right hand, but can also have a different target
	 */
	UFUNCTION()
	void FlyToTarget(const FQuat& TargetRotation, const FVector& TargetLocation, float MinSpeed, float FlightMaxDuration);

	void OnBellHit();


	/**
	 * Function called in Tick if the Bell is in the PostLaunch state.
	 */
	void PostLaunchTick();

	/**
	 * Function called in Tick if the Bell is in the FlyToTarget state.
	 */
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

	enum class EBellState : uint8
	{
		EBS_None,
		/**
		 * State when the Bell is idle. Used when a player is holding the Ritual Bell.
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
	};

	UPROPERTY(EditDefaultsOnly, Category="J|Bell|Breakable object")
	TSubclassOf<AActor> BreakableObjectClass;

	FFlightInfo FlightInfo{};

	EBellState BellState{EBellState::EBS_Idle};

	UPROPERTY(EditAnywhere, Category="J|Bell|Properties")
	float FlightSpeed{500.f};

	UPROPERTY(EditAnywhere, Category="J|Bell|Properties")
	float MaxDistance{200.f};

	UPROPERTY(EditAnywhere, Category="J|Bell|Properties")
	FRotator BellThrowRotationRate{-1440.f, 0.f, 0.f};

	/**
	 * Represents the minimum speed at which the bell can be recalled to its target.
	 */
	UPROPERTY(EditAnywhere, Category="J|Bell|Properties")
	float BellRecallMinSpeed{1600.f};

	/**
	 * Represents the maximum duration, in seconds, the bell can spend flying back to its recall target.
	 */
	UPROPERTY(EditAnywhere, Category="J|Bell|Properties")
	float BellRecallMaxDuration{1.f};

	FVector InitialLocation{};

	FTimerHandle IdleAfterImpactTimer;

	/**
	 * Represents the duration, in seconds, that the bell remains idle after impacting a surface
	 * before transitioning to the next state or performing a subsequent action.
	 */
	UPROPERTY(EditAnywhere, Category="J|Bell|Properties")
	float IdleAfterImpactDuration{0.5f};

	UPROPERTY(VisibleAnywhere, Category="J|Bell|Mesh")
	TObjectPtr<UStaticMeshComponent> BellMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;

#pragma endregion
};
