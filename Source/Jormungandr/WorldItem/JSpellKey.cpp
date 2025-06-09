// Fill out your copyright notice in the Description page of Project Settings.


#include "JSpellKey.h"


AJSpellKey::AJSpellKey()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AJSpellKey::Collect_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT(__FUNCTION__))
	if (bActive)
	{
		bActive = false;
		return true;
	}
	return false;
}
