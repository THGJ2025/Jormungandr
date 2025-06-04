// Fill out your copyright notice in the Description page of Project Settings.


#include "JPlayer.h"
#include "Jormungandr/InteractionSystem/JInteractComponent.h"
#include "Jormungandr/InventoryItem/JInventoryComponent.h"


AJPlayer::AJPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractComponent = CreateDefaultSubobject<UJInteractComponent>(TEXT("Interact Component"));

	InventoryComponent = CreateDefaultSubobject<UJInventoryComponent>(TEXT("Inventory Component"));
}
