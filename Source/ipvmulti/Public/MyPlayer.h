// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayer.generated.h"

UCLASS()
class IPVMULTI_API AMyPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPlayer();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	float number;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FColor color;

	virtual void Tick(float DeltaTime) override;
};
