// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Motores/Door.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ipvmulti/ipvmultiCharacter.h"
#include "ipvmulti/Public/Motores/Widgets/KeyMessageWidget.h"
#include "ipvmulti/Public/Motores/Widgets/WinWidget.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = DoorMesh;

    TriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComp"));
    TriggerComp->InitBoxExtent(FVector(100.f, 100.f, 200.f));
    TriggerComp->SetupAttachment(RootComponent);
    TriggerComp->SetCollisionProfileName("Trigger");

    TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);

    SolidCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SolidCollision"));
    SolidCollision->SetupAttachment(RootComponent);
    SolidCollision->SetCollisionProfileName("BlockAllDynamic");
    SolidCollision->SetBoxExtent(FVector(50.f, 100.f, 200.f));
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                          bool bFromSweep, const FHitResult& SweepResult)
{
    if (AipvmultiCharacter* Player = Cast<AipvmultiCharacter>(OtherActor))
    {
        if (Player->GetCurrentKeys() >= RequiredKeys)
        {
            OpenDoor();
            Player->UseKeys(RequiredKeys);
        }
        else
        {
            if (UKeyMessageWidget* MessageWidget = CreateWidget<UKeyMessageWidget>(GetWorld(), Player->KeyMessageWidgetClass))
            {
                MessageWidget->AddToViewport();
                MessageWidget->ShowMissingKeyMessage(RequiredKeys - Player->GetCurrentKeys());
            }
        }
    }
}

void ADoor::OpenDoor()
{
    DoorMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
    GetWorld()->GetTimerManager().SetTimer(
        WinWidgetTimerHandle,     
        this,                     
        &ADoor::ShowWinWidgetDelayed, 
        2.0f,                     
        false                   
    );
}

void ADoor::ShowWinWidgetDelayed()
{
    if (AipvmultiCharacter* Player = Cast<AipvmultiCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        if (UWinWidget* WinWidget = CreateWidget<UWinWidget>(GetWorld(), Player->WinWidgetClass))
        {
            WinWidget->ShowWinScreen();
        }
    }
}


