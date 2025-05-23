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

    // Configuración de la cámara
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Configuración de gameplay
    TurnRateGamepad = 45.f;
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
    FireRate = 0.25f;
    bIsFiringWeapon = false;
    CurrentKeys = 0;

    SetReplicatingMovement(true);
}

void AipvmultiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    // Configuración de inputs
    PlayerInputComponent->BindAxis("MoveForward", this, &AipvmultiCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AipvmultiCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AipvmultiCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AipvmultiCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AipvmultiCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AipvmultiCharacter::StopFire);
}

void AipvmultiCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AipvmultiCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AipvmultiCharacter::TurnAtRate(float Rate)
{
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

void AipvmultiCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void AipvmultiCharacter::OnHealthUpdate()
{
    // Mostrar vida en pantalla
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vida: %.0f"), CurrentHealth));

    if (CurrentHealth <= 0.f)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("¡Jugador muerto!"));
        DisableInput(GetWorld()->GetFirstPlayerController());
        SetLifeSpan(2.0f);
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

float AipvmultiCharacter::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, 
                                   AController* EventInstigator, AActor* DamageCauser)
{
    if (DamageTaken <= 0.f || CurrentHealth <= 0.f)
    {
        return 0.f;
    }

    if (HasAuthority())
    {
        float NewHealth = CurrentHealth - DamageTaken;
        SetCurrentHealth(NewHealth);
        
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, 
            FString::Printf(TEXT("¡Recibiste %.0f de daño!"), DamageTaken));
    }

    return DamageTaken;
}

void AipvmultiCharacter::RespawnPlayer()
{
    CurrentHealth = MaxHealth;
    OnHealthUpdate();
    EnableInput(GetWorld()->GetFirstPlayerController());
    // Lógica adicional de respawn aquí
}

void AipvmultiCharacter::StartFire()
{
    if (!bIsFiringWeapon)
    {
        bIsFiringWeapon = true;
        HandleFire();
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
            const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.f, 0.f, 50.f));

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            World->SpawnActor<AThirdPersonMPProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
        }
    }
}

void AipvmultiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AipvmultiCharacter, CurrentHealth);
    DOREPLIFETIME(AipvmultiCharacter, CurrentKeys);
}

