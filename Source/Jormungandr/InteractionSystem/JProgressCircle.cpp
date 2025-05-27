// Fill out your copyright notice in the Description page of Project Settings.


#include "JProgressCircle.h"
#include "Components/Image.h"
#include "Jormungandr/Utility/WeredMacros.h"
#include "Kismet/KismetMaterialLibrary.h"

void UJProgressCircle::NativePreConstruct()
{
	Super::NativePreConstruct();

	UVALID_LOG_DEBUG(ProgressCircleMatInst)
	UVALID_LOG_DEBUG(ProgressCircleImage)

	ProgressCircleMatDyn = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, ProgressCircleMatInst);
	ProgressCircleImage->SetBrushFromMaterial(ProgressCircleMatDyn);
	SetPercent(Percent);
	SetBackgroundCircleOpacity(BackgroundCircleOpacity);
	SetImageSize(ImageSize);
}

void UJProgressCircle::SetPercent(const float InPercent) const
{
	UVALID_LOG_DEBUG(ProgressCircleMatDyn)

	ProgressCircleMatDyn->SetScalarParameterValue(FName("Percent"), FMath::Clamp(InPercent, 0.f, 1.f));
}

void UJProgressCircle::SetOpacity(const float InOpacity) const
{
	UVALID_LOG_DEBUG(ProgressCircleMatDyn)

	ProgressCircleMatDyn->SetScalarParameterValue(FName("Opacity"), FMath::Clamp(InOpacity, 0.f, 1.f));
}

void UJProgressCircle::SetImageSize(const FVector2D InImageSize) const
{
	UVALID_LOG_DEBUG(ProgressCircleImage)

	ProgressCircleImage->SetDesiredSizeOverride(InImageSize);
}

void UJProgressCircle::SetBackgroundCircleOpacity(const float InOpacity) const
{
	UVALID_LOG_DEBUG(ProgressCircleMatDyn)

	ProgressCircleMatDyn->SetScalarParameterValue(FName("BackgroundCircleOpacity"), InOpacity);
}
