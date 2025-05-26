// Fill out your copyright notice in the Description page of Project Settings.


#include "JItemManagerComponent.h"
#include "JUsableItem.h"


UJItemManagerComponent::UJItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UJItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Items.IsValidIndex(0))
	{
		ActiveItem = Items[0];
	}
}

void UJItemManagerComponent::AddItem(AJUsableItem* NewItem)
{
	Items.Add(NewItem);
	NewItem->SetOwner(GetOwner());

	if (!ActiveItem)
	{
		ActiveItem = Items[0];
	}
}

void UJItemManagerComponent::NextItem()
{
	CurrentItemIndex = (CurrentItemIndex + 1) % Items.Num();

	ActiveItem = Items[CurrentItemIndex];
}

void UJItemManagerComponent::PreviousItem()
{
	CurrentItemIndex = (CurrentItemIndex - 1 + Items.Num()) % Items.Num();

	ActiveItem = Items[CurrentItemIndex];
}

void UJItemManagerComponent::PrimaryUseActiveItem()
{
	if (!ActiveItem)
	{
		return;
	}

	ActiveItem->PrimaryUse();
}

void UJItemManagerComponent::SecondaryUseActiveItem()
{
	if (!ActiveItem)
	{
		return;
	}

	ActiveItem->SecondaryUse();
}
