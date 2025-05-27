// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JProgressCircle.generated.h"

class UImage;

UCLASS()
class JORMUNGANDR_API UJProgressCircle : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	/**
	 * Sets the percentage of the foreground circle.
	 */
	UFUNCTION(BlueprintCallable, Category="J")
	void SetPercent(const float InPercent) const;

	/**
	 * Sets the opacity of the foreground circle.
	 */
	UFUNCTION(BlueprintCallable, Category="J")
	void SetOpacity(const float InOpacity) const;

	UFUNCTION(BlueprintCallable, Category="J")
	void SetImageSize(const FVector2D InImageSize) const;
	UFUNCTION(BlueprintCallable, Category="J")
	void SetBackgroundCircleOpacity(const float InOpacity) const;

private:
	/**
	 * Percentage of the foreground circle.
	 */
	UPROPERTY(EditAnywhere, meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"), Category="J")
	float Percent{0.5f};
	/**
	 * Opacity of the foreground circle.
	 */
	UPROPERTY(EditAnywhere, meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"), Category="J")
	float Opacity{1.f};
	UPROPERTY(EditAnywhere, Category="J")
	FVector2D ImageSize{32.f, 32.f};
	UPROPERTY(EditAnywhere, meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"), Category="J")
	float BackgroundCircleOpacity{0.2f};

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ProgressCircleMatDyn;
	UPROPERTY(EditDefaultsOnly, Category="J")
	TObjectPtr<UMaterialInstance> ProgressCircleMatInst;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ProgressCircleImage;
};
