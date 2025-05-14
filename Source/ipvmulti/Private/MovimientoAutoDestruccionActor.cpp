#include "ipvmulti/Public/MovimientoAutoDestruccionActor.h"
#include "TimerManager.h"

AMovimientoAutoDestruccionActor::AMovimientoAutoDestruccionActor()
{
	PrimaryActorTick.bCanEverTick = true; // Habilitar Tick si vamos a mover el actor
}

void AMovimientoAutoDestruccionActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AMovimientoAutoDestruccionActor::DestruirActor,
		TiempoDeVida,
		false
	);
}

void AMovimientoAutoDestruccionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	// Mover el actor hacia adelante
	AddActorLocalOffset(FVector(Velocidad * DeltaTime, 0, 0));
}

void AMovimientoAutoDestruccionActor::DestruirActor()
{
	Destroy();
}


