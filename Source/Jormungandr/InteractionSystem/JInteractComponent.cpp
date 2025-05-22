// Fill out your copyright notice in the Description page of Project Settings.


#include "JInteractComponent.h"


UJInteractComponent::UJInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UJInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UJInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
