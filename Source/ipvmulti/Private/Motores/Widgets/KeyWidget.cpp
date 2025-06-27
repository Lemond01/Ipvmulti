// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Motores/Widgets/KeyWidget.h"
#include "Components/TextBlock.h"

void UKeyWidget::UpdateKeys(int32 Count)
{
	if (KeyCountText)
	{
		KeyCountText->SetText(FText::AsNumber(Count));
	}
}
