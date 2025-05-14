// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class IPVMULTI_API AMyBullet : public AActor
{
	GENERATED_BODY()
    
public:
	AMyBullet();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float Damage = 10.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
};
