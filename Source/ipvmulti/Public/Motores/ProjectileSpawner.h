#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileSpawner.generated.h"

class UArrowComponent;
class AMyBullet;  // Cambiado a AMyBullet

UCLASS()
class IPVMULTI_API AProjectileSpawner : public AActor
{
	GENERATED_BODY()
    
public:
	AProjectileSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;  // Nuevo para limpieza

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* DirectionArrow;

	// Usamos AMyBullet en lugar de AThirdPersonMPProjectile
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AMyBullet> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float SpawnInterval = 2.0f;

	UFUNCTION(BlueprintCallable)
	void StartSpawning();

	UFUNCTION(BlueprintCallable)
	void StopSpawning();

private:
	FTimerHandle SpawnTimerHandle;
	void SpawnProjectile();
};
