// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn_01.generated.h"

UCLASS()
class IPVMULTI_API AMyPawn_01 : public APawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AMyPawn_01();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void VerticalAxis(float Value);
	void HorizontalAxis(float Value);
	
	

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Velocity = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 100.0f;
	

};
