// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Motores/OtherPlayer.h"
#include "ipvmulti/Public/Motores/GM_Base.h"
#include "ipvmulti/Public/Motores/UserWidget_01.h" 

// Sets default values
AOtherPlayer::AOtherPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOtherPlayer::BeginPlay()
{
	Super::BeginPlay();
	/*
	if (GetController() && GetController() -> IsLocalController() && gameHudClass)
	{
		UUserWidget_01* gameHud = CreateWidget<UUserWidget_01>(GetWorld(), gameHudClass);
		if (gameHud)
		{
			AAGM_Base* gameMode = CreateWidget<>UUGameHud>(GetWorld(), gameHudClass);
			if (gameMode)
			{
				gameHud -> myGameMode = gameMode;
			}
			gameHud -> AddToViewport();
			gameHud -> UpdateScore();
		}
	}
	*/
	
}

// Called every frame
void AOtherPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AOtherPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

