// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyPickup.generated.h"

class USphereComponent;

UCLASS()
class IPVMULTI_API AKeyPickup : public AActor
{
	GENERATED_BODY()
    
public:
	AKeyPickup();

protected:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComp;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
					   bool bFromSweep, const FHitResult& SweepResult);
};