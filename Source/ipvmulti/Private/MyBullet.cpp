// Fill out your copyright notice in the Description page of Project Settings.
#include "ipvmulti/Public/MyBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ipvmulti/Public/MyPlayer.h"

AMyBullet::AMyBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	// Configuración de colisión
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyBullet::OnHit);
	RootComponent = CollisionComp;

	// Configuración de movimiento
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// Autodestrucción después de 3 segundos
	InitialLifeSpan = 3.0f;
}

void AMyBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AMyBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if (AMyPlayer* Player = Cast<AMyPlayer>(OtherActor))
		{
			Player->ReceiveDamage(Damage);
		}
		Destroy();
	}
}

