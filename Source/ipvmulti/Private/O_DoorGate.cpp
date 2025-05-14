// Fill out your copyright notice in the Description page of Project Settings.
#include "ipvmulti/Public/O_DoorGate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
AO_DoorGate::AO_DoorGate()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(50.f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AO_DoorGate::OnOverlapBegin);
}

void AO_DoorGate::BeginPlay()
{
	Super::BeginPlay();

	// Si esta puerta no ha generado otras
	if (PuertasGeneradas.Num() == 0)
	{
		// Generar las otras puertas en el mundo
		for (int32 i = 1; i < NumTotalPuertas; ++i)
		{
			// Posicionar las puertas de manera separada en el mundo
			FVector SpawnLocation = GetActorLocation() + FVector(i * 200.0f, 0.0f, 0.0f);  // Cambia la separación si es necesario
			FRotator SpawnRotation = GetActorRotation();

			// Generar la puerta
			AO_DoorGate* NuevaPuerta = GetWorld()->SpawnActor<AO_DoorGate>(AO_DoorGate::StaticClass(), SpawnLocation, SpawnRotation);
			PuertasGeneradas.Add(NuevaPuerta);

			// Asignar la puerta como falsa
			if (NuevaPuerta)
			{
				NuevaPuerta->SetIsRealGate(false);
			}
		}

		// Generar la puerta verdadera
		int32 RandomIndex = FMath::RandRange(0, PuertasGeneradas.Num() - 1);
		PuertasGeneradas[RandomIndex]->SetIsRealGate(true);
	}
}

void AO_DoorGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AO_DoorGate::SetIsRealGate(bool bReal)
{
	bIsRealGate = bReal;
}

void AO_DoorGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								  bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (bIsRealGate)
		{
			// Inicia temporizador para destruir la puerta
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AO_DoorGate::DestroyGate, DelayBeforeDestroy, false);
		}
		else
		{
			// Puedes poner un sonido, efecto, o mensaje aquí si quieres
			UE_LOG(LogTemp, Warning, TEXT("¡Puerta falsa!"));
		}
	}
}

void AO_DoorGate::DestroyGate()
{
	Destroy();
}
