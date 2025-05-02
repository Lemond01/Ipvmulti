// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/MyEnemy.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyEnemy::Tick(float DeltaTime)
{
	AddActorLocalOffset(FVector(velocity,0,0));
}

