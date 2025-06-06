// Fill out your copyright notice in the Description page of Project Settings.


#include "JBracelet.h"


AJBracelet::AJBracelet()
{
	PrimaryActorTick.bCanEverTick = false;

	BraceletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bracelet Mesh"));
	SetRootComponent(BraceletMesh);
	BraceletMesh->SetCollisionProfileName(FName("NoCollision"));
}

void AJBracelet::BeginPlay()
{
	Super::BeginPlay();
}

void AJBracelet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
