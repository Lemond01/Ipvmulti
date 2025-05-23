// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThirdPersonMPProjectile.generated.h"

UCLASS()
class IPVMULTI_API AThirdPersonMPProjectile : public AActor
{
	GENERATED_BODY()
 
public:
	AThirdPersonMPProjectile();
 
protected:
	virtual void BeginPlay() override;
 
public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ExplosionEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage;
 
protected:
 
	virtual void Destroyed() override;
 
	UFUNCTION(Category = "Projectile")
	void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
 
};
