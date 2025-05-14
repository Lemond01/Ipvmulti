#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovimientoGiroActor.generated.h"

UCLASS()
class IPVMULTI_API AMovimientoGiroActor : public AActor
{
	GENERATED_BODY()

public:
	AMovimientoGiroActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float Velocidad = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float VelocidadRotacion = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destrucción")
	float TiempoParaDestruir = 5.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle TimerHandle;
	void DestruirActor();
};


