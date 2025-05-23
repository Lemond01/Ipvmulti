// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/OthersActors/MyPawn_01.h"

// Sets default values
AMyPawn_01::AMyPawn_01()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPawn_01::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn_01::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputComponent -> BindAxis("Vertical", this, &AMyPawn_01::VerticalAxis);
	InputComponent -> BindAxis("Horizontal", this, &AMyPawn_01::HorizontalAxis);

}

void AMyPawn_01::VerticalAxis(float Value)
{
	float deltaTime = GetWorld()->GetDeltaSeconds();
	AddActorLocalOffset(FVector(Value * Velocity * deltaTime, 0, 0));
}

void AMyPawn_01::HorizontalAxis(float Value)
{
	float deltaTime = GetWorld()->GetDeltaSeconds();
	FRotator Rotation = FRotator(0, Value * RotationSpeed * deltaTime, 0);
	AddActorLocalRotation(Rotation);
}
