#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerActor.generated.h"

UCLASS()
class IPVMULTI_API ASpawnerActor : public AActor
{
	GENERATED_BODY()
    
public:    
	ASpawnerActor();

	// Clases de los actores a spawnear
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	TSubclassOf<AActor> Clase1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	TSubclassOf<AActor> Clase2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	TSubclassOf<AActor> Clase3;

	// Tiempo entre spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	float TiempoEntreSpawns = 2.0f;

	// Tiempo después del último spawn para autodestruirse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	float TiempoParaDestruirse = 1.0f;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnearObjeto1();
	void SpawnearObjeto2();
	void SpawnearObjeto3();
	void Autodestruirse();
};
