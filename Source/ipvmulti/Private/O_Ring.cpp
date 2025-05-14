#include "ipvmulti/Public/O_Ring.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

AO_Ring::AO_Ring()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(100.f);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AO_Ring::OnOverlapBegin);
	CollisionSphere->SetCollisionProfileName(TEXT("Trigger"));
	
	bReplicates = true;
	SetReplicateMovement(true);
}

void AO_Ring::BeginPlay()
{
	Super::BeginPlay();

	if (bDebugSpawnPoints)
	{
		for (const FVector& Location : SpawnLocations)
		{
			DrawDebugSphere(GetWorld(), Location, 50.f, 12, FColor::Green, true, -1.f, 0, 2.f);
		}
	}
}

void AO_Ring::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (OverlapSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OverlapSound, GetActorLocation());
		}

		if (HasAuthority())
		{
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AO_Ring::DestroySelf, DelayBeforeDestruction, false);
		}
		else
		{
			Server_DestroySelf();
		}
	}
}

void AO_Ring::Server_DestroySelf_Implementation()
{
	if (!HasAuthority()) return;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AO_Ring::DestroySelf, DelayBeforeDestruction, false);
}

void AO_Ring::DestroySelf()
{
	if (bHasRespawned) return;
	bHasRespawned = true;

	if (DestroyParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyParticles, GetActorLocation());
	}

	RespawnRing();
	Destroy();
}

void AO_Ring::RespawnRing()
{
	if (SpawnLocations.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No hay SpawnLocations definidas"));
		return;
	}

	int32 NewIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
	FVector NewLocation = SpawnLocations[NewIndex];
	FRotator NewRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AO_Ring* NewRing = GetWorld()->SpawnActor<AO_Ring>(GetClass(), NewLocation, NewRotation, SpawnParams);

	if (NewRing)
	{
		NewRing->SpawnLocations = this->SpawnLocations;
		NewRing->DelayBeforeDestruction = this->DelayBeforeDestruction;
		NewRing->OverlapSound = this->OverlapSound;
		NewRing->DestroyParticles = this->DestroyParticles;
	}
}



