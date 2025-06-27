// Fill out your copyright notice in the Description page of Project Settings.

#include "ipvmulti/Public/Actors/ThirdPersonMPProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"

AThirdPersonMPProjectile::AThirdPersonMPProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    SphereComponent->InitSphereRadius(37.5f);
    SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = SphereComponent;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    StaticMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    if (DefaultMesh.Succeeded())
    {
        StaticMesh->SetStaticMesh(DefaultMesh.Object);
        StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
        StaticMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
    if (DefaultExplosionEffect.Succeeded())
    {
        ExplosionEffect = DefaultExplosionEffect.Object;
    }

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
    ProjectileMovementComponent->InitialSpeed = 1500.0f;
    ProjectileMovementComponent->MaxSpeed = 1500.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

    DamageType = UDamageType::StaticClass();
    Damage = 10.0f;
}

void AThirdPersonMPProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    if (HasAuthority())
    {
        SphereComponent->OnComponentHit.AddDynamic(this, &AThirdPersonMPProjectile::OnProjectileImpact);
    }
}
 
void AThirdPersonMPProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
 
void AThirdPersonMPProjectile::Destroyed()
{
    FVector spawnLocation = GetActorLocation();
    UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, 
                                           FRotator::ZeroRotator, true, 
                                           EPSCPoolMethod::AutoRelease);
}
 
void AThirdPersonMPProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, 
                                                AActor* OtherActor, 
                                                UPrimitiveComponent* OtherComp, 
                                                FVector NormalImpulse, 
                                                const FHitResult& Hit)
{
    if (OtherActor && OtherActor != GetInstigator())
    {
        AController* InstigatorController = GetInstigator() ? GetInstigator()->GetController() : nullptr;
        UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, 
                                         InstigatorController, this, DamageType);
    }
    Destroy();
}