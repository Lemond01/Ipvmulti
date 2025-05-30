#include "ipvmulti/Public/Motores/ObstacleProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "ipvmulti/ipvmultiCharacter.h"

AObstacleProjectile::AObstacleProjectile()
{
    // Configuración de replicación
    bReplicates = true;
    SetReplicateMovement(true);

    // Componente de colisión
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(15.0f);
    CollisionComp->SetCollisionProfileName("Projectile");
    CollisionComp->SetGenerateOverlapEvents(true);
    CollisionComp->SetNotifyRigidBodyCollision(true);
    CollisionComp->OnComponentHit.AddDynamic(this, &AObstacleProjectile::OnHit);
    RootComponent = CollisionComp;

    // Componente de movimiento
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f; // Sin gravedad

    // Configuración por defecto para evitar que el proyectil sea afectado por el tiempo de juego
    ProjectileMovement->bAutoActivate = true;
    PrimaryActorTick.bCanEverTick = false;
}

void AObstacleProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Configurar la velocidad inicial
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
    }
}

void AObstacleProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority()) return;

    // Aplicar daño si golpea a un jugador
    if (OtherActor && OtherActor != this)
    {
        AipvmultiCharacter* Player = Cast<AipvmultiCharacter>(OtherActor);
        if (Player)
        {
            UGameplayStatics::ApplyDamage(
                Player,
                DamageAmount,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }

    // Efectos de impacto (se replican a todos los clientes)
    if (ImpactParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ImpactParticles,
            GetActorLocation(),
            GetActorRotation(),
            true,
            EPSCPoolMethod::AutoRelease
        );
    }

    if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            ImpactSound,
            GetActorLocation()
        );
    }

    // Destruir el proyectil
    Destroy();
}