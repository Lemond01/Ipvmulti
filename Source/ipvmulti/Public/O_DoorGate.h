// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "O_DoorGate.generated.h"

UCLASS()
class IPVMULTI_API AO_DoorGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AO_DoorGate();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Establece si es la puerta verdadera
	void SetIsRealGate(bool bReal);

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroyGate();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Gate")
	float DelayBeforeDestroy = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Gate")
	int32 NumTotalPuertas = 4;  // Número total de puertas a generar

	UPROPERTY(VisibleAnywhere, Category = "Gate")
	TArray<AO_DoorGate*> PuertasGeneradas; // Array para almacenar las puertas generadas

	bool bIsRealGate = false;

	FTimerHandle DestroyTimerHandle;
};
