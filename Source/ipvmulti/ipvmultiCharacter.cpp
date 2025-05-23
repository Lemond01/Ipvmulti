// Copyright 1998-2022 Epic Games, Inc. All Rights Reserved.

#include "ipvmultiCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "ipvmulti/Public/Actors/ThirdPersonMPProjectile.h"

AipvmultiCharacter::AipvmultiCharacter()
{
    // Activa la replicación para esta clase
    bReplicates = true;

    // Crear y configurar el brazo para la cámara
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // Distancia de la cámara al personaje
    CameraBoom->bUsePawnControlRotation = true; // Que rote con el control del jugador

    // Crear y configurar la cámara
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; // La cámara no rota independiente

    TurnRateGamepad = 45.f; // Valor por defecto para giro con gamepad

    // Valores por defecto de salud
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;

    FireRate = 0.25f;
    bIsFiringWeapon = false;

    // Replicación para variables
    SetReplicatingMovement(true);
}

void AipvmultiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicar CurrentHealth para que se sincronice entre servidor y clientes
    DOREPLIFETIME(AipvmultiCharacter, CurrentHealth);
}

void AipvmultiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    // Movimiento
    PlayerInputComponent->BindAxis("MoveForward", this, &AipvmultiCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AipvmultiCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AipvmultiCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AipvmultiCharacter::LookUpAtRate);

    // Salto
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    // Disparo
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AipvmultiCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AipvmultiCharacter::StopFire);
}

void AipvmultiCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // Encontrar la dirección hacia adelante
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Dirección adelante en el mundo
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AipvmultiCharacter::MoveRight(float Value)
{
    if ( (Controller != nullptr) && (Value != 0.0f) )
    {
        // Encontrar dirección derecha
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Dirección derecha en el mundo
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AipvmultiCharacter::TurnAtRate(float Rate)
{
    // gira con tasa dependiente del frame rate
    AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AipvmultiCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AipvmultiCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    StartFire();
}

void AipvmultiCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopFire();
}

// --- Salud ---

void AipvmultiCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void AipvmultiCharacter::OnHealthUpdate()
{
    // Aquí puedes poner lógica para actualizar UI, efectos visuales, sonido, etc.
    if (CurrentHealth <= 0.f)
    {
        // Lógica de muerte (por ejemplo deshabilitar input, animaciones, etc.)
        // Esto queda a tu implementación personalizada
    }
}

void AipvmultiCharacter::SetCurrentHealth(float healthValue)
{
    if (HasAuthority())
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
        OnHealthUpdate();
    }
}

float AipvmultiCharacter::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (DamageTaken <= 0.f || CurrentHealth <= 0.f)
    {
        return 0.f;
    }

    if (HasAuthority())
    {
        float NewHealth = CurrentHealth - DamageTaken;
        SetCurrentHealth(NewHealth);
    }

    return DamageTaken;
}

// --- Disparo ---

void AipvmultiCharacter::StartFire()
{
    if (!bIsFiringWeapon)
    {
        bIsFiringWeapon = true;
        HandleFire();

        // Iniciar timer para repetir disparo con FireRate
        GetWorldTimerManager().SetTimer(FiringTimer, this, &AipvmultiCharacter::HandleFire, FireRate, true);
    }
}

void AipvmultiCharacter::StopFire()
{
    bIsFiringWeapon = false;
    GetWorldTimerManager().ClearTimer(FiringTimer);
}

void AipvmultiCharacter::HandleFire_Implementation()
{
    if (ProjectileClass != nullptr)
    {
        UWorld* const World = GetWorld();
        if (World != nullptr)
        {
            const FRotator SpawnRotation = GetControlRotation();
            // La posición donde se dispara puede ajustarse (por ejemplo frente al personaje)
            const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.f, 0.f, 50.f));

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // Spawn el proyectil en el mundo
            World->SpawnActor<AThirdPersonMPProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
        }
    }
}

