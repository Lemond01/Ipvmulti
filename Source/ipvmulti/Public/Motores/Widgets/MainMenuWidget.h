// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class IPVMULTI_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LoadGameLevel(FName LevelName = TEXT("L_Motores"));

	UFUNCTION(BlueprintCallable)
	void QuitGame();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlayButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;
	

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnQuitClicked();
};
