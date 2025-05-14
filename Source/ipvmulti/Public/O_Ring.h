// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "O_Ring.generated.h"

UCLASS()
class IPVMULTI_API AO_Ring : public AActor
{
	GENERATED_BODY()

public:
	AO_Ring();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroySelf();

	void RespawnRing();
	
	UFUNCTION(Server, Reliable)
	void Server_DestroySelf();

	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<FVector> SpawnLocations;

	int32 CurrentSpawnIndex = 0;

public:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, Category = "Destruction")
	float DelayBeforeDestruction = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* OverlapSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DestroyParticles;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugSpawnPoints = true;

private:
	bool bHasRespawned = false;
};

