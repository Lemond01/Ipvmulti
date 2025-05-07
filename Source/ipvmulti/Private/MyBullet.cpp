// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/MyBullet.h"

// Sets default values
AMyBullet::AMyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(5,0,0));

}

