// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class USoundBase;

UCLASS()
class IPVMULTI_API ADamageZone : public AActor
{
	GENERATED_BODY()
    
public:    
	ADamageZone();

protected:
	virtual void BeginPlay() override;

	/** Componente de colisión */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* DamageArea;

	/** Efecto de partículas */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* DamageParticles;

	/** Sonido de daño continuo */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* DamageSound;

	/** Daño por segundo */
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float DamagePerSecond = 10.0f;

	/** Intervalo entre aplicaciones de daño */
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float DamageInterval = 0.5f;

	/** Actores actualmente dentro de la zona */
	TArray<AActor*> OverlappingActors;

	/** Temporizador para daño periódico */
	FTimerHandle DamageTimerHandle;

	/** Aplicar daño a todos los actores en la zona */
	UFUNCTION()
	void ApplyPeriodicDamage();

	/** Manejar entrada de actores */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Manejar salida de actores */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
