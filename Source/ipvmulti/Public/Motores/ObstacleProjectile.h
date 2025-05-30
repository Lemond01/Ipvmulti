#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ObstacleProjectile.generated.h"

class USphereComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class IPVMULTI_API AObstacleProjectile : public AActor
{
	GENERATED_BODY()
    
public:    
	AObstacleProjectile();

protected:
	virtual void BeginPlay() override;

	/** Componente de colisión esférico */
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	/** Componente de movimiento del proyectil */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	/** Efecto de partículas al impactar */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactParticles;

	/** Sonido al impactar */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* ImpactSound;

	/** Daño que causa el proyectil */
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float DamageAmount = 10.0f;

	/** Velocidad inicial del proyectil */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float InitialSpeed = 3000.0f;

	/** Velocidad máxima del proyectil */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 3000.0f;

public:    
	/** Llamado cuando el proyectil colisiona con algo */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Devuelve el componente de colisión */
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Devuelve el componente de movimiento del proyectil */
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};