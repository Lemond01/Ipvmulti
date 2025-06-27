// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinWidget.generated.h"

UCLASS()
class IPVMULTI_API UWinWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowWinScreen();

protected:
	FTimerHandle WinTimerHandle;

	UFUNCTION()
	void ReturnToMainMenu();
};
