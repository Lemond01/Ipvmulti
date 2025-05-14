// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "O_Hexagon.generated.h"

UCLASS()
class IPVMULTI_API AO_Hexagon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AO_Hexagon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HideHexagon();

	UFUNCTION()
	void ShowHexagon();

	FTimerHandle HideTimerHandle;
	FTimerHandle RespawnTimerHandle;

public:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

	// Tiempo de espera antes de ocultarse
	UPROPERTY(EditAnywhere, Category = "Destruction")
	float DelayBeforeHide = 1.5f;

	// Tiempo antes de reaparecer
	UPROPERTY(EditAnywhere, Category = "Respawn")
	float DelayBeforeRespawn = 3.0f;
};
