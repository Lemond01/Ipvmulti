// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Actors/AmmoPickup.h"
#include "Components/SphereComponent.h"
#include "ipvmulti/ipvmultiCharacter.h"

AAmmoPickup::AAmmoPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;

	// Configuración de replicación
	SetReplicateMovement(true);
}

void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
    
	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlapBegin);
	}
}

void AAmmoPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AipvmultiCharacter* Character = Cast<AipvmultiCharacter>(OtherActor);
		if (Character)
		{
			Character->AddAmmo(AmmoAmount);
			Destroy();
		}
	}
}

