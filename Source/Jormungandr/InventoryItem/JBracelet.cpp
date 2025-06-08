// Fill out your copyright notice in the Description page of Project Settings.


#include "JBracelet.h"


AJBracelet::AJBracelet()
{
	PrimaryActorTick.bCanEverTick = true;

	BraceletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bracelet Mesh"));
	SetRootComponent(BraceletMesh);
	BraceletMesh->SetCollisionProfileName(FName("NoCollision"));
}

void AJBracelet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CurrentCharges = MaxCharges;
}

void AJBracelet::BeginPlay()
{
	Super::BeginPlay();
}

void AJBracelet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (BraceletState)
	{
	case EBraceletState::EBS_None:
		break;
	case EBraceletState::EBS_SlowCharge:
		AddCharge(DeltaTime * SlowChargingAmount);
		break;
	case EBraceletState::EBS_FastCharge:
		AddCharge(DeltaTime * FastChargingAmount);
		break;
	case EBraceletState::EBS_Max:
		break;
	}
}

void AJBracelet::AddCharge(const float Amount)
{
	if (CurrentCharges >= MaxCharges)
	{
		return;
	}

	CurrentCharges += Amount;
	if (CurrentCharges > MaxCharges)
	{
		CurrentCharges = MaxCharges;
	}

	OnChargeChangedDelegate.Broadcast(CurrentCharges);
}

bool AJBracelet::SubCharge(const float Amount)
{
	if (CurrentCharges - Amount < 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: CurrentCharges: %f, AmountToSub: %f"), TEXT(__FUNCTION__), CurrentCharges, Amount)
		return false;
	}

	CurrentCharges -= Amount;

	OnChargeChangedDelegate.Broadcast(CurrentCharges);

	return true;
}
