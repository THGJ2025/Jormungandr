// Fill out your copyright notice in the Description page of Project Settings.


#include "JPickUpItem.h"
#include "Jormungandr/InteractionSystem/JInteractableComponent.h"
#include "Jormungandr/InventoryItem/JInventoryComponent.h"
#include "Jormungandr/InventoryItem/JInventoryItem.h"
#include "Jormungandr/Player/JPlayer.h"
#include "Jormungandr/Utility/WeredMacros.h"


AJPickUpItem::AJPickUpItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AJPickUpItem::InteractPressTriggered_Implementation()
{
	Super::InteractPressTriggered_Implementation();

	UVALID_LOG_DEBUG(ItemToPickUp)

	AJPlayer* Player{Cast<AJPlayer>(GetInteractableComponent()->GetInteractInstigator())};

	UVALID_LOG_DEBUG(Player)

	Player->GetInventoryComponent()->AddItem(ItemToPickUp);

	Destroy();
}
