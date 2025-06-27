// Fill out your copyright notice in the Description page of Project Settings.


#include "Motores/Widgets/AmmoWidget.h"
#include "Components/TextBlock.h"

void UAmmoWidget::UpdateAmmo(int32 Current, int32 Max, int32 Reserve)
{
    if (CurrentAmmoText) 
    {
        CurrentAmmoText->SetText(FText::AsNumber(Current));
    }
    if (MaxAmmoText)
    {
        MaxAmmoText->SetText(FText::AsNumber(Max));
    }
    if (ReserveAmmoText)
    {
        ReserveAmmoText->SetText(FText::AsNumber(Reserve));
    }
}
