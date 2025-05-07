// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/MyEnemy.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(timerHandle, this, &AMyEnemy::ShootBullet, timer, true);
	GetWorldTimerManager().SetTimer(stopTimerHandle, this, &AMyEnemy::StopShootBullet, stopTimer, false);
}

void AMyEnemy::ShootBullet()
{
	if (spawnObject)
	{
		FVector spawnLocation = GetActorLocation();
		FRotator spawnRotation = GetActorRotation();
		GetWorld()->SpawnActor<AActor>(spawnObject, spawnLocation, spawnRotation);	
	}
}

void AMyEnemy::StopShootBullet()
{
	GetWorldTimerManager().ClearTimer(timerHandle);
}