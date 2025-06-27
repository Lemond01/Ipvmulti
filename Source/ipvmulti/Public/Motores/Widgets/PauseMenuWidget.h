// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

UCLASS()
class IPVMULTI_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ResumeGame();

	UFUNCTION(BlueprintCallable)
	void QuitToMainMenu();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;

	virtual void NativeConstruct() override;
};