// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

UCLASS()
class IPVMULTI_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void UpdateAmmo(int32 Current, int32 Max, int32 Reserve);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentAmmoText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MaxAmmoText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ReserveAmmoText;
};