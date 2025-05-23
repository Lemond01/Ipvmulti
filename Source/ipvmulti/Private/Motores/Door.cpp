// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/Motores/Door.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ipvmulti/ipvmultiCharacter.h"


// Sets default values
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
    
	// Trigger para apertura (ya existente)
	TriggerComp->SetBoxExtent(FVector(100.f, 100.f, 200.f));
	TriggerComp->SetCollisionProfileName("Trigger");
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
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, 
				FString::Printf(TEXT("Necesitas %d llaves más"), RequiredKeys - Player->GetCurrentKeys()));
		}
	}
}

void ADoor::OpenDoor()
{
	// Implementa animación o rotación aquí
	DoorMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

