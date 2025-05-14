#include "ipvmulti/Public/MovimientoActor.h"
#include "TimerManager.h"

AMovimientoActor::AMovimientoActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovimientoActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AMovimientoActor::Autodestruirse,
		TiempoDeVida,
		false
	);
}

void AMovimientoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector(Velocidad * DeltaTime, 0, 0));
}

void AMovimientoActor::Autodestruirse()
{
	Destroy();
}



