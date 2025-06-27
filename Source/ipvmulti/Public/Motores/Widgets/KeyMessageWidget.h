// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyMessageWidget.generated.h"

UCLASS()
class IPVMULTI_API UKeyMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowMissingKeyMessage(int32 KeysNeeded);

	UFUNCTION(BlueprintCallable)
	void HideMessage();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MessageText;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float DisplayTime = 3.0f;

	FTimerHandle MessageTimer;
};
