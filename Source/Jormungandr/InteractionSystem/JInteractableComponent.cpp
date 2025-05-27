// Fill out your copyright notice in the Description page of Project Settings.


#include "JInteractableComponent.h"
#include "JInteractableActorBase.h"
#include "JInteractIconWidget.h"
#include "Components/WidgetComponent.h"
#include "Jormungandr/Utility/WeredMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteractableComponent, Log, All);

UJInteractableComponent::UJInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeInteractWidget();
	UpdateWidgetAttachment();
}

void UJInteractableComponent::AddOutlineMeshComponent(UMeshComponent* MeshComponent)
{
	OutlineMeshComps.Add(MeshComponent);
}

void UJInteractableComponent::SetOutlineMeshComponents(const TArray<UMeshComponent*>& NewOutlineMeshComponents)
{
	OutlineMeshComps = NewOutlineMeshComponents;
}

void UJInteractableComponent::DisplayInteractionUI(const bool bShowInteract)
{
	UVALID_LOG_DEBUG(InteractWidget)

	InteractWidget->SetVisibility(bShowInteract ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	UpdateOutline(bShowInteract);
}

void UJInteractableComponent::InteractPressStarted(AActor* OtherInstigator)
{
	InteractInstigator = OtherInstigator;
	UE_LOG(LogTemp, Warning, TEXT("PressStarted"))
}

void UJInteractableComponent::InteractPressTriggered()
{
	UE_LOG(LogTemp, Warning, TEXT("PressTriggered"))
}

void UJInteractableComponent::InteractPressCompleted()
{
	InteractInstigator = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("PressCompleted"))
}

float UJInteractableComponent::InteractHoldStarted(AActor* OtherInstigator)
{
	UVALID_LOG_DEBUGF(InteractWidget)

	InteractInstigator = OtherInstigator;
	UE_LOG(LogTemp, Warning, TEXT("HoldStarted"))

	return HoldTimeThreshold;
}

void UJInteractableComponent::InteractHoldOngoing(const float ElapsedSeconds)
{
	UVALID_LOG_DEBUG(InteractWidget)

	const float ProgressFraction{FMath::Clamp(ElapsedSeconds / HoldTimeThreshold, 0.f, 1.f)};
	InteractWidget->SetIconOpacity(ProgressFraction);
	InteractWidget->SetProgressCircleOpacity(ProgressFraction);
	InteractWidget->SetProgressCirclePercent(ProgressFraction);
}

void UJInteractableComponent::InteractHoldTriggered()
{
	UE_LOG(LogTemp, Warning, TEXT("HoldTriggered"))
}

void UJInteractableComponent::InteractHoldCanceled()
{
	UVALID_LOG_DEBUG(InteractWidget)

	InteractInstigator = nullptr;
	InteractWidget->SetProgressCircleOpacity(0.f);
	InteractWidget->SetProgressCirclePercent(0.f);
	InteractWidget->SetIconOpacity(0.f);
	UE_LOG(LogTemp, Warning, TEXT("HoldCanceled"))
}

void UJInteractableComponent::InteractHoldCompleted()
{
	InteractInstigator = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("HoldCompleted"))
}

bool UJInteractableComponent::DoesUseCustomInteractArea() const
{
	return bUseCustomInteractArea;
}

void UJInteractableComponent::SetCanInteract(const bool bNewCanInteract)
{
	bCanInteract = bNewCanInteract;
}

bool UJInteractableComponent::GetCanInteract() const
{
	return bCanInteract;
}

EInteractType UJInteractableComponent::GetInteractType() const
{
	return InteractType;
}

void UJInteractableComponent::SetInteractCategory(const EInteractCategory InInteractCategory)
{
	UVALID_LOG_DEBUG(InteractWidget)

	InteractWidget->SetInteractCategory(InInteractCategory);
}

void UJInteractableComponent::OnRegister()
{
	Super::OnRegister();

	AActor* Owner{GetOwner()};
	if (!Cast<AJInteractableActorBase>(Owner))
	{
		UE_LOG(LogInteractableComponent, Error, TEXT("%s owned by %s must register a component owned by a InteractableActorBase"), *GetName(), *GetNameSafe(GetOwner()))
	}
}

void UJInteractableComponent::InitializeInteractWidget()
{
	UVALID_LOG_DEBUG(InteractWidgetClass)

	InteractWidget = CreateWidget<UJInteractIconWidget>(GetWorld(), InteractWidgetClass);
	InteractWidgetComp = NewObject<UWidgetComponent>(this, TEXT("InteractWidgetComp"));

	UVALID_LOG_DEBUG(InteractWidget)
	UVALID_LOG_DEBUG(InteractWidgetComp)

	InteractWidget->Init(InteractCategory,
	                     InteractType,
	                     MinimalIconOpacity,
	                     IconSize,
	                     MinimalProgressCircleOpacity,
	                     ProgressCircleSize);
	InteractWidget->SetVisibility(ESlateVisibility::Hidden);
	InteractWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidgetComp->SetDrawAtDesiredSize(true);
	InteractWidgetComp->SetWidget(InteractWidget);
	InteractWidgetComp->RegisterComponent();
}

void UJInteractableComponent::UpdateWidgetAttachment() const
{
	UVALID_LOG_DEBUG(InteractWidgetComp)

	USceneComponent* WidgetAttachComp{IJInteractInterface::Execute_GetWidgetAttachmentComponent(GetOwner())};
	if (!WidgetAttachComp)
	{
		WidgetAttachComp = GetOwner()->GetRootComponent();
	}
	InteractWidgetComp->AttachToComponent(WidgetAttachComp, FAttachmentTransformRules::KeepRelativeTransform);
}

void UJInteractableComponent::UpdateOutline(const bool bHighlight)
{
	for (UMeshComponent* OutlineMeshComponent : OutlineMeshComps)
	{
		if (OutlineMeshComponent)
		{
			OutlineMeshComponent->SetRenderCustomDepth(bHighlight);
			OutlineMeshComponent->SetCustomDepthStencilValue(bHighlight ? 1 : 0);
		}
	}
}
