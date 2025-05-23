// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/Motores/KeyPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ipvmulti/ipvmultiCharacter.h"

// Sets default values
AKeyPickup::AKeyPickup()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(50.f);
	CollisionComp->SetCollisionProfileName("OverlapAllDynamic");
	RootComponent = CollisionComp;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AKeyPickup::OnOverlapBegin);
}

void AKeyPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
							   bool bFromSweep, const FHitResult& SweepResult)
{
	if (AipvmultiCharacter* Player = Cast<AipvmultiCharacter>(OtherActor))
	{
		Player->AddKey();
		Destroy();
	}
}

