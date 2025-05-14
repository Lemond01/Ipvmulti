// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/O_Hexagon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AO_Hexagon::AO_Hexagon()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(50.f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AO_Hexagon::OnOverlapBegin);
}

void AO_Hexagon::BeginPlay()
{
	Super::BeginPlay();    
}

void AO_Hexagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AO_Hexagon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// Inicia el temporizador para ocultarlo
		GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &AO_Hexagon::HideHexagon, DelayBeforeHide, false);
	}
}

void AO_Hexagon::HideHexagon()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AO_Hexagon::ShowHexagon, DelayBeforeRespawn, false);
}

void AO_Hexagon::ShowHexagon()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

