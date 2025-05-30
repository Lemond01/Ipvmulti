#include "ipvmulti/Public/Motores/ProjectileSpawner.h"
#include "Components/ArrowComponent.h"
#include "ipvmulti/Public/Motores/ObstacleProjectile.h"

AProjectileSpawner::AProjectileSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
	RootComponent = DirectionArrow;
	DirectionArrow->ArrowSize = 2.0f;
	DirectionArrow->SetHiddenInGame(false);
}

void AProjectileSpawner::BeginPlay()
{
	Super::BeginPlay();
	StartSpawning();
}

void AProjectileSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopSpawning();  // Limpia el timer al destruirse
	Super::EndPlay(EndPlayReason);
}

void AProjectileSpawner::StartSpawning()
{
	if (ProjectileClass && !SpawnTimerHandle.IsValid())  // Verificación adicional
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AProjectileSpawner::SpawnProjectile, SpawnInterval, true);
	}
}

void AProjectileSpawner::StopSpawning()
{
	if (SpawnTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void AProjectileSpawner::SpawnProjectile()
{
	if (!ProjectileClass || !GetWorld()) return;

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = DirectionArrow->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AObstacleProjectile* Projectile = GetWorld()->SpawnActor<AObstacleProjectile>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (Projectile)
	{
		// Debug visual
		DrawDebugDirectionalArrow(GetWorld(), 
			SpawnLocation, 
			SpawnLocation + DirectionArrow->GetForwardVector() * 500.f,
			50.f, FColor::Green, false, 1.5f);
	}
}
