// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEnemy.generated.h"

UCLASS()
class IPVMULTI_API AMyEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemy();
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float velocity = 10.0f;


	virtual void Tick(float DeltaTime) override;


};
