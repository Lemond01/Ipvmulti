#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovimientoActor.generated.h"

UCLASS()
class IPVMULTI_API AMovimientoActor : public AActor
{
	GENERATED_BODY()
    
public:    
	AMovimientoActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movimiento")
	float Velocidad = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Destrucción")
	float TiempoDeVida = 5.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle TimerHandle;
	void Autodestruirse();
};


