// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ipvmulti/public/Motores/UserWidget_01.h"
#include "GameFramework/Pawn.h" 
#include "OtherPlayer.generated.h"


UCLASS()
class IPVMULTI_API AOtherPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOtherPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget_01> GameHudClass;
};
