// Fill out your copyright notice in the Description page of Project Settings.


#include "ipvmulti/Public/Motores/DamageZone.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "ipvmulti/ipvmultiCharacter.h"

ADamageZone::ADamageZone()
{
    PrimaryActorTick.bCanEverTick = false;

    // Configuración de replicación
    bReplicates = true;

    // Componente de colisión
    DamageArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageArea"));
    DamageArea->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    DamageArea->SetGenerateOverlapEvents(true);
    DamageArea->InitBoxExtent(FVector(100.f, 100.f, 50.f));
    RootComponent = DamageArea;

    // Componente de partículas
    DamageParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamageParticles"));
    DamageParticles->SetupAttachment(RootComponent);
    DamageParticles->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    DamageParticles->bAutoActivate = true;

    // Configuración inicial
    DamageInterval = 0.5f;
    DamagePerSecond = 10.0f;
}

void ADamageZone::BeginPlay()
{
    Super::BeginPlay();

    // Configurar delegados de overlap
    DamageArea->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnBeginOverlap);
    DamageArea->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnEndOverlap);

    // Iniciar temporizador de daño (solo en servidor)
    if (HasAuthority())
    {
        GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ADamageZone::ApplyPeriodicDamage, DamageInterval, true);
    }
}

void ADamageZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    AipvmultiCharacter* Player = Cast<AipvmultiCharacter>(OtherActor);
    if (Player && !OverlappingActors.Contains(Player))
    {
        OverlappingActors.Add(Player);
        
        // Debug
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, 
            FString::Printf(TEXT("Jugador %s entró en zona de daño"), *Player->GetName()));
    }
}

void ADamageZone::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!HasAuthority()) return;

    AipvmultiCharacter* Player = Cast<AipvmultiCharacter>(OtherActor);
    if (Player && OverlappingActors.Contains(Player))
    {
        OverlappingActors.Remove(Player);
        
        // Debug
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, 
            FString::Printf(TEXT("Jugador %s salió de zona de daño"), *Player->GetName()));
    }
}

void ADamageZone::ApplyPeriodicDamage()
{
    if (!HasAuthority()) return;

    float DamageToApply = DamagePerSecond * DamageInterval;

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->CanBeDamaged())
        {
            AipvmultiCharacter* Player = Cast<AipvmultiCharacter>(Actor);
            if (Player)
            {
                UGameplayStatics::ApplyDamage(
                    Player,
                    DamageToApply,
                    nullptr,
                    this,
                    UDamageType::StaticClass()
                );
            }
        }
    }
}

