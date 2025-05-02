// Fill out your copyright notice in the Description page of Project Settings.
#include "ipvmulti/Public/MyPlayer.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyPlayer::Tick(float DeltaTime)
{
	AddActorLocalOffset(FVector(10,0,0));
}


