// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Motores/UserWidget_01.h"
#include "ipvmulti/Public/Motores/GM_Base.h" 

void UUserWidget_01::NativeConstruct()
{
	Super::NativeConstruct();

	if (!myGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("myGameMode is NULL"));
		return;
	}

	if (!scoreLabel)
	{
		UE_LOG(LogTemp, Warning, TEXT("scoreLabel is NULL"));
		return;
	}

	FString scoreString = FString::FromInt(myGameMode->counter);
	scoreLabel->SetText(FText::FromString(scoreString));
	if (scoreButon)
	{
		scoreButon->OnClicked.AddDynamic(this, &UUserWidget_01::RaiseScore);
	}
	UpdateScore();
}

void UUserWidget_01::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!myGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("myGameMode is NULL"));
		return;
	}
}

void UUserWidget_01::UpdateScore()
{
	if (!scoreLabel && myGameMode)
	{
		FString scoreString = FString::FromInt(myGameMode->counter);
		FText scoreText = FText::FromString(scoreString);
		scoreLabel-> SetText(scoreText);
	}
}

void UUserWidget_01::RaiseScore()
{
	if (true)
	{
		myGameMode->counter += 10;
		UpdateScore();
	}
}


