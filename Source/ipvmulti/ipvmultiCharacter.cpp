// Copyright 1998-2025 Epic Games, Inc. All Rights Reserved.

#include "ipvmultiCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "ipvmulti/Public/Actors/ThirdPersonMPProjectile.h"

AipvmultiCharacter::AipvmultiCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Activate replication
    bReplicates = true;
    SetReplicateMovement(true);

    // Camera setup
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Gameplay setup
    TurnRateGamepad = 45.f;
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
    MaxAmmo = 5;
    CurrentAmmo = MaxAmmo;
    FireRate = 0.25f;
    bIsFiringWeapon = false;
    CurrentKeys = 0;
}

void AipvmultiCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (IsLocallyControlled() && AmmoWidgetClass)
    {
        AmmoWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), AmmoWidgetClass);
        if (AmmoWidgetInstance)
        {
            AmmoWidgetInstance->AddToViewport();
            UpdateAmmoWidget();
        }
    }
}


void AipvmultiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    // Inputs setup
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

void AipvmultiCharacter::OnHealthUpdate_Implementation()
{
    if (GEngine)
    {
        FString HealthText = FString::Printf(TEXT("Health: %.1f/%.1f"), CurrentHealth, MaxHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, HealthText);
    }

    if (CurrentHealth <= 0.f)
    {
        DisableInput(GetWorld()->GetFirstPlayerController());
    }
}

void AipvmultiCharacter::AddAmmo(int32 Amount)
{
    if (HasAuthority())
    {
        CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, 0, MaxAmmo);
        OnRep_CurrentAmmo();
    }
}

void AipvmultiCharacter::UseAmmo(int32 Amount)
{
    if (HasAuthority())
    {
        CurrentAmmo = FMath::Max(0, CurrentAmmo - Amount);
        OnRep_CurrentAmmo();
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
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, 
                FString::Printf(TEXT("Damage Taken: %.0f"), DamageTaken));
        }
    }

    return DamageTaken;
}

void AipvmultiCharacter::RespawnPlayer()
{
    if (HasAuthority())
    {
        CurrentHealth = MaxHealth;
        CurrentAmmo = MaxAmmo;
        OnHealthUpdate();
        OnRep_CurrentAmmo();
        EnableInput(GetWorld()->GetFirstPlayerController());
    }
}

void AipvmultiCharacter::StartFire()
{
    if (!bIsFiringWeapon && CurrentAmmo > 0)
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

void AipvmultiCharacter::OnRep_CurrentAmmo()
{
    UpdateAmmoWidget();
}

void AipvmultiCharacter::UpdateAmmoWidget()
{
    if (AmmoWidgetInstance && IsLocallyControlled())
    {
        FString Command = FString::Printf(TEXT("UpdateAmmo %d %d"), CurrentAmmo, MaxAmmo);
        AmmoWidgetInstance->CallFunctionByNameWithArguments(*Command, nullptr, nullptr, true);
    }
}

void AipvmultiCharacter::HandleFire()
{
    if (CurrentAmmo <= 0)
    {
        StopFire();
        return;
    }

    if (GetLocalRole() < ROLE_Authority)
    {
        Server_HandleFire();
    }
    else
    {
        Server_HandleFire_Implementation();
    }
}

bool AipvmultiCharacter::Server_HandleFire_Validate()
{
    return ProjectileClass != nullptr && CurrentAmmo > 0;
}

void AipvmultiCharacter::Server_HandleFire_Implementation()
{
    if (ProjectileClass && GetWorld() && CurrentAmmo > 0)
    {
        const FRotator SpawnRotation = GetControlRotation();
        const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.f, 0.f, 50.f));

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        GetWorld()->SpawnActor<AThirdPersonMPProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
        
        UseAmmo(1); // Consume 1 ammo
    }
}

void AipvmultiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AipvmultiCharacter, CurrentHealth);
    DOREPLIFETIME(AipvmultiCharacter, MaxHealth);
    DOREPLIFETIME(AipvmultiCharacter, CurrentKeys);
    DOREPLIFETIME(AipvmultiCharacter, bIsFiringWeapon);
    DOREPLIFETIME(AipvmultiCharacter, CurrentAmmo);
    DOREPLIFETIME(AipvmultiCharacter, MaxAmmo);
}
