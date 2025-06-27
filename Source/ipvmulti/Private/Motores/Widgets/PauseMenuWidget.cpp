// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Motores/Widgets/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void UPauseMenuWidget::NativeConstruct()
{
Super::NativeConstruct();

if (ResumeButton)
{
ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
}

if (QuitButton)
{
QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
}
}

void UPauseMenuWidget::ResumeGame()
{
APlayerController* PC = GetOwningPlayer();
if (PC)
{
RemoveFromParent();
PC->SetPause(false);
PC->bShowMouseCursor = false;
PC->SetInputMode(FInputModeGameOnly());
}
}

void UPauseMenuWidget::QuitToMainMenu()
{
UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}