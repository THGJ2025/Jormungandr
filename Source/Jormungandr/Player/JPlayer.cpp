// Fill out your copyright notice in the Description page of Project Settings.


#include "JPlayer.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Jormungandr/Items/Usable/JItemManagerComponent.h"
#include "Jormungandr/Utility/WeredMacros.h"


AJPlayer::AJPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemManagerComponent = CreateDefaultSubobject<UJItemManagerComponent>(TEXT("Item Manager"));
}

void AJPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UVALID_LOG_DEBUG(PrimaryUseActiveItemAction)
	UVALID_LOG_DEBUG(SecondaryUseActiveItemAction)

	UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(PlayerInputComponent)};

	EnhancedInputComponent->BindAction(PrimaryUseActiveItemAction, ETriggerEvent::Started, this, &AJPlayer::PrimaryUseActiveItem);
	EnhancedInputComponent->BindAction(SecondaryUseActiveItemAction, ETriggerEvent::Started, this, &AJPlayer::SecondaryUseActiveItem);
}

void AJPlayer::PrimaryUseActiveItem()
{
	ItemManagerComponent->PrimaryUseActiveItem();
}

void AJPlayer::SecondaryUseActiveItem()
{
	ItemManagerComponent->SecondaryUseActiveItem();
}
