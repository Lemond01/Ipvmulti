// Fill out your copyright notice in the Description page of Project Settings.
#include "ipvmulti/Public/MyPlayer.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configuración de colisión
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionComp->SetCollisionProfileName("Pawn");
	RootComponent = CollisionComp;
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayer::ReceiveDamage(float DamageAmount)
{
	Health -= DamageAmount;

	// Mostrar vida en pantalla
	FString HealthString = FString::Printf(TEXT("Vida: %.1f"), Health);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HealthString);

	if (Health <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡Jugador Derrotado!"));
		// Destroy(); // Opcional: Destruir al jugador al morir
	}
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



