// Fill out your copyright notice in the Description page of Project Settings.


#include "JPlayer.h"


AJPlayer::AJPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AJPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AJPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

