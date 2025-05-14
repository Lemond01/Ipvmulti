#include "ipvmulti/Public/MovimientoGiroActor.h"

AMovimientoGiroActor::AMovimientoGiroActor()
{
	PrimaryActorTick.bCanEverTick = true; // Habilita Tick()
}

void AMovimientoGiroActor::BeginPlay()
{
	Super::BeginPlay();

	// Configurar temporizador para autodestrucción
	if (TiempoParaDestruir > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AMovimientoGiroActor::DestruirActor,
			TiempoParaDestruir,
			false // No repetir
		);
	}
}

void AMovimientoGiroActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Movimiento hacia adelante
	FVector Direccion = GetActorForwardVector();
	AddActorWorldOffset(Direccion * Velocidad * DeltaTime, true);

	// Rotación continua
	FRotator Rotacion(0.0f, VelocidadRotacion * DeltaTime, 0.0f);
	AddActorWorldRotation(Rotacion);
}

void AMovimientoGiroActor::DestruirActor()
{
	Destroy(); // Destruye este actor
}



