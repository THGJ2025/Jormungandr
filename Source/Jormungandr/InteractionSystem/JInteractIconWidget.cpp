// Fill out your copyright notice in the Description page of Project Settings.


#include "JInteractIconWidget.h"
#include "JProgressCircle.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Jormungandr/Utility/WeredMacros.h"


void UJInteractIconWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UVALID_LOG_DEBUG(IconSwitcher)
	UVALID_LOG_DEBUG(CollectIcon)
	UVALID_LOG_DEBUG(OpenIcon)
	UVALID_LOG_DEBUG(UseIcon)
	UVALID_LOG_DEBUG(UnlockIcon)
	UVALID_LOG_DEBUG(ProgressCircle)

	SetInteractCategory(InteractCategory);

	switch (InteractType)
	{
	case EInteractType::EIT_Press:
		ProgressCircle->SetVisibility(ESlateVisibility::Collapsed);
		SetIconOpacity(1.f);
		break;
	case EInteractType::EIT_Hold:
		ProgressCircle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetIconOpacity(0.f);
		SetProgressCircleOpacity(0.f);
		SetProgressCirclePercent(0.f);
		SetProgressCircleSize(ProgressCircleSize);
		break;
	}

	SetIconSize(IconSize);
}

void UJInteractIconWidget::Init(const EInteractCategory NewInteractCategory,
                                const EInteractType NewInteractType,
                                const float NewMinimalIconOpacity,
                                const FVector2D NewIconSize,
                                const float NewMinimalProgressCircleOpacity,
                                const FVector2D NewProgressCircleSize)
{
	InteractCategory = NewInteractCategory;
	InteractType = NewInteractType;
	MinimalIconOpacity = NewMinimalIconOpacity;
	IconSize = NewIconSize;
	MinimalProgressCircleOpacity = NewMinimalProgressCircleOpacity;
	ProgressCircleSize = NewProgressCircleSize;
}

void UJInteractIconWidget::SetInteractCategory(const EInteractCategory InInteractCategory)
{
	switch (InInteractCategory)
	{
	case EInteractCategory::EIC_Use:
		IconSwitcher->SetActiveWidget(UseIcon);
		break;
	case EInteractCategory::EIC_Collect:
		IconSwitcher->SetActiveWidget(CollectIcon);
		break;
	case EInteractCategory::EIC_Open:
		IconSwitcher->SetActiveWidget(OpenIcon);
		break;
	case EInteractCategory::EIC_Unlock:
		IconSwitcher->SetActiveWidget(UnlockIcon);
		break;
	}
}

void UJInteractIconWidget::SetIconSize(const FVector2D Size) const
{
	UVALID_LOG_DEBUG(IconSwitcher)

	for (UWidget* Icon : IconSwitcher->GetAllChildren())
	{
		if (UImage* ImageIcon{Cast<UImage>(Icon)})
		{
			ImageIcon->SetDesiredSizeOverride(Size);
		}
	}
}

void UJInteractIconWidget::SetIconOpacity(const float Opacity)
{
	UVALID_LOG_DEBUG(IconSwitcher)

	const float ClampedOpacity{FMath::Clamp(Opacity, 0.f, 1.f)};
	CurrentIconOpacity = FMath::Lerp(MinimalIconOpacity, 1.f, ClampedOpacity);

	IconSwitcher->SetRenderOpacity(CurrentIconOpacity);
}

void UJInteractIconWidget::SetProgressCircleSize(const FVector2D Size) const
{
	UVALID_LOG_DEBUG(ProgressCircle)

	ProgressCircle->SetImageSize(Size);
}

void UJInteractIconWidget::SetProgressCircleOpacity(const float Opacity)
{
	UVALID_LOG_DEBUG(ProgressCircle)

	const float ClampedOpacity{FMath::Clamp(Opacity, 0.f, 1.f)};
	CurrentProgressCircleOpacity = FMath::Lerp(MinimalProgressCircleOpacity, 1.f, ClampedOpacity);
	ProgressCircle->SetOpacity(CurrentProgressCircleOpacity);
}

void UJInteractIconWidget::SetProgressCirclePercent(const float Percent)
{
	UVALID_LOG_DEBUG(ProgressCircle)

	const float ClampedPercent{FMath::Clamp(Percent, 0.f, 1.f)};
	CurrentProgressCirclePercent = ClampedPercent;
	ProgressCircle->SetPercent(CurrentProgressCirclePercent);
}
