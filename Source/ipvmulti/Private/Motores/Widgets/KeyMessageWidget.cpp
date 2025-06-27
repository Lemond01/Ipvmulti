// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/Motores/Widgets/KeyMessageWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UKeyMessageWidget::ShowMissingKeyMessage(int32 KeysNeeded)
{
	if (MessageText)
	{
		FString Message = FString::Printf(TEXT("¡Te faltan %d llaves para entrar!"), KeysNeeded);
		MessageText->SetText(FText::FromString(Message));
		SetVisibility(ESlateVisibility::Visible);
        
		GetWorld()->GetTimerManager().SetTimer(
			MessageTimer, 
			this, 
			&UKeyMessageWidget::HideMessage, 
			DisplayTime, 
			false
		);
	}
}

void UKeyMessageWidget::HideMessage()
{
	SetVisibility(ESlateVisibility::Hidden);
}
