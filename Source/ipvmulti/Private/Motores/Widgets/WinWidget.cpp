// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/Motores/Widgets/WinWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void UWinWidget::ShowWinScreen()
{
	AddToViewport(100);
	GetWorld()->GetTimerManager().SetTimer(WinTimerHandle, this, &UWinWidget::ReturnToMainMenu, 5.0f, false);
}

void UWinWidget::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}