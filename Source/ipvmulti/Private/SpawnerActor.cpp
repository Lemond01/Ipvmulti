#include "ipvmulti/Public/SpawnerActor.h"
#include "Engine/World.h"
#include "TimerManager.h"

ASpawnerActor::ASpawnerActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	// Spawnear el primer actor inmediatamente
	SpawnearObjeto1();

	// Configurar timers para los siguientes spawns
	FTimerHandle TimerHandle2;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle2, 
		this, 
		&ASpawnerActor::SpawnearObjeto2, 
		TiempoEntreSpawns, 
		false
	);

	FTimerHandle TimerHandle3;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle3, 
		this, 
		&ASpawnerActor::SpawnearObjeto3, 
		TiempoEntreSpawns * 2, 
		false
	);

	// Configurar timer para autodestrucción
	FTimerHandle TimerDestruccion;
	GetWorld()->GetTimerManager().SetTimer(
		TimerDestruccion,
		this,
		&ASpawnerActor::Autodestruirse,
		TiempoEntreSpawns * 2 + TiempoParaDestruirse,
		false
	);
}

void ASpawnerActor::SpawnearObjeto1()
{
	if (Clase1)
	{
		GetWorld()->SpawnActor<AActor>(Clase1, GetActorLocation(), GetActorRotation());
	}
}

void ASpawnerActor::SpawnearObjeto2()
{
	if (Clase2)
	{
		GetWorld()->SpawnActor<AActor>(Clase2, GetActorLocation(), GetActorRotation());
	}
}

void ASpawnerActor::SpawnearObjeto3()
{
	if (Clase3)
	{
		GetWorld()->SpawnActor<AActor>(Clase3, GetActorLocation(), GetActorRotation());
	}
}

void ASpawnerActor::Autodestruirse()
{
	Destroy();
}

