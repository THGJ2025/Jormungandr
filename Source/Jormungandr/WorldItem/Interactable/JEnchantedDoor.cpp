// Fill out your copyright notice in the Description page of Project Settings.


#include "JEnchantedDoor.h"


AJEnchantedDoor::AJEnchantedDoor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AJEnchantedDoor::Open_Implementation()
{
	bIsOpen = true;
}

