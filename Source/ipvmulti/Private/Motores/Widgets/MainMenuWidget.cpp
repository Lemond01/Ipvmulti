// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Motores/Widgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
}

void UMainMenuWidget::OnPlayClicked()
{
	LoadGameLevel();
}

void UMainMenuWidget::OnQuitClicked()
{
	QuitGame();
}

void UMainMenuWidget::LoadGameLevel(FName LevelName)
{
	// Asegúrate de que el nivel existe
	if (!UGameplayStatics::DoesSaveGameExist("L_Motores", 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("El nivel L_Motores no existe!"));
		return;
	}
    
	UGameplayStatics::OpenLevel(GetWorld(), "L_Motores");
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false
	);
}