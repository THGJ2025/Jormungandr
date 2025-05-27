// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JInteractInterface.h"
#include "Blueprint/UserWidget.h"
#include "JInteractIconWidget.generated.h"

class UImage;
class UWidgetSwitcher;
class UJProgressCircle;

UCLASS()
class JORMUNGANDR_API UJInteractIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	void Init(const EInteractCategory NewInteractCategory,
	          const EInteractType NewInteractType,
	          const float NewMinimalIconOpacity,
	          const FVector2D NewIconSize,
	          const float NewMinimalProgressCircleOpacity,
	          const FVector2D NewProgressCircleSize);

	/**
	 * Configures the active widget of the icon switcher based on the specified interaction category.
	 *
	 * @param InInteractCategory The category of interaction to determine the active icon widget.
	 */
	void SetInteractCategory(const EInteractCategory InInteractCategory);
	void SetIconSize(const FVector2D Size) const;
	/**
	 * Sets the opacity of the interaction icon by interpolating between the minimal and 1.f opacity.
	 * To decrease @see DecreaseIconOpacity
	 *
	 * @see MinimalIconOpacity
	 */
	void SetIconOpacity(const float Opacity);

	void SetProgressCircleSize(const FVector2D Size) const;
	/**
	 * Sets the opacity of the progress circle by interpolating between the minimal and 1.f opacity.
	 * To decrease @see DecreaseProgressCircleOpacity
	 *
	 * @see MinimalProgressCircleOpacity
	 */
	void SetProgressCircleOpacity(const float Opacity);
	/**
	 * Updates the progress circle's fill percentage.
	 *
	 * @param Percent Range: 0.f to 1.f.
	 */
	void SetProgressCirclePercent(const float Percent);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="J|Interact")
	EInteractCategory InteractCategory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="J|Interact")
	EInteractType InteractType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="J|Interact")
	FVector2D IconSize{50.f, 50.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true, UIMin="0", UIMax="1", ClampMin="0", ClampMax="1", EditCondition="InteractType==EInteractType::EIT_Hold"), Category="J|Interact")
	float MinimalIconOpacity{0.3f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true, EditCondition="InteractType==EInteractType::EIT_Hold"), Category="J|Interact")
	FVector2D ProgressCircleSize{100.f, 100.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true, UIMin="0", UIMax="1", ClampMin="0", ClampMax="1", EditCondition="InteractType==EInteractType::EIT_Hold"), Category="J|Interact")
	float MinimalProgressCircleOpacity{0.f};

	UPROPERTY(VisibleAnywhere, Category="J|Interact")
	float CurrentIconOpacity{};
	UPROPERTY(VisibleAnywhere, Category="J|Interact")
	float CurrentProgressCircleOpacity{};
	UPROPERTY(VisibleAnywhere, Category="J|Interact")
	float CurrentProgressCirclePercent{};

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true), Category="J|Interact")
	TObjectPtr<UJProgressCircle> ProgressCircle;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true), Category="J|Interact")
	TObjectPtr<UWidgetSwitcher> IconSwitcher;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true), Category="J|Interact")
	TObjectPtr<UImage> CollectIcon;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true), Category="J|Interact")
	TObjectPtr<UImage> OpenIcon;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true), Category="J|Interact")
	TObjectPtr<UImage> UseIcon;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true), Category="J|Interact")
	TObjectPtr<UImage> UnlockIcon;
};
