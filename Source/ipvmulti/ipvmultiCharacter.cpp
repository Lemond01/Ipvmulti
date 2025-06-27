// Fill out your copyright notice in the Description page of Project Settings.

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
#include "ipvmulti/Public/Motores/Widgets/AmmoWidget.h"
#include "ipvmulti/Public/Motores/Widgets/MainMenuWidget.h"
#include "ipvmulti/Public/Motores/Widgets/PauseMenuWidget.h"
#include "ipvmulti/Public/Motores/Widgets/WinWidget.h"
#include "ipvmulti/Public/Motores/Widgets/KeyWidget.h"
#include "ipvmulti/Public/Motores/Widgets/KeyMessageWidget.h"

AipvmultiCharacter::AipvmultiCharacter()
{
    // Configuración del tamaño de la cápsula de colisión
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Configuración de la replicación
    bReplicates = true;
    SetReplicateMovement(true);

    // Configuración de la cámara
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Configuración del gameplay
    TurnRateGamepad = 45.f;
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
    MaxAmmo = 5;
    CurrentAmmo = MaxAmmo;
    MaxReserveAmmo = 30;
    ReserveAmmo = 0;
    FireRate = 0.25f;
    bIsFiringWeapon = false;
    CurrentKeys = 0;
    CurrentPauseWidget = nullptr;
}

void AipvmultiCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Crear widgets UI si es controlado localmente
    if (IsLocallyControlled())
    {
        // Widget de munición
        if (AmmoWidgetClass)
        {
            AmmoWidgetInstance = CreateWidget<UAmmoWidget>(GetWorld(), AmmoWidgetClass);
            if (AmmoWidgetInstance)
            {
                AmmoWidgetInstance->AddToViewport();
                UpdateAmmoWidget();
            }
        }

        // Widget de llaves
        if (KeyWidgetClass)
        {
            KeyWidgetInstance = CreateWidget<UKeyWidget>(GetWorld(), KeyWidgetClass);
            if (KeyWidgetInstance)
            {
                KeyWidgetInstance->AddToViewport();
                KeyWidgetInstance->UpdateKeys(CurrentKeys);
            }
        }
    }
}

void AipvmultiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Configuración de los inputs de movimiento
    PlayerInputComponent->BindAxis("MoveForward", this, &AipvmultiCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AipvmultiCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AipvmultiCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AipvmultiCharacter::LookUpAtRate);

    // Configuración de las acciones
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AipvmultiCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AipvmultiCharacter::StopFire);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AipvmultiCharacter::Reload);
    PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AipvmultiCharacter::TogglePauseMenu);
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
        ReserveAmmo = MaxReserveAmmo;
        OnHealthUpdate();
        OnRep_CurrentAmmo();
        OnRep_ReserveAmmo();
        EnableInput(GetWorld()->GetFirstPlayerController());
    }
}

void AipvmultiCharacter::AddAmmoToReserve(int32 Amount)
{
    if (HasAuthority())
    {
        ReserveAmmo = FMath::Clamp(ReserveAmmo + Amount, 0, MaxReserveAmmo);
        OnRep_ReserveAmmo();
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

void AipvmultiCharacter::Reload()
{
    if (HasAuthority() && CurrentAmmo < MaxAmmo && ReserveAmmo > 0)
    {
        int32 NeededAmmo = MaxAmmo - CurrentAmmo;
        int32 AmmoToTransfer = FMath::Min(NeededAmmo, ReserveAmmo);
        
        CurrentAmmo += AmmoToTransfer;
        ReserveAmmo -= AmmoToTransfer;
        
        OnRep_CurrentAmmo();
        OnRep_ReserveAmmo();

        // Podrías añadir aquí sonido/animación de recarga
    }
}

void AipvmultiCharacter::OnRep_CurrentAmmo()
{
    UpdateAmmoWidget();
}

void AipvmultiCharacter::OnRep_ReserveAmmo()
{
    UpdateAmmoWidget();
}

void AipvmultiCharacter::UpdateAmmoWidget()
{
    if (AmmoWidgetInstance)
    {
        AmmoWidgetInstance->UpdateAmmo(CurrentAmmo, MaxAmmo, ReserveAmmo);
    }
}

void AipvmultiCharacter::OnRep_CurrentKeys()
{
    if (KeyWidgetInstance && IsLocallyControlled())
    {
        KeyWidgetInstance->UpdateKeys(CurrentKeys);
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
        
        UseAmmo(1);
    }
}

void AipvmultiCharacter::ShowMainMenu()
{
    if (MainMenuWidgetClass && IsLocallyControlled())
    {
        if (UMainMenuWidget* MainMenu = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass))
        {
            MainMenu->AddToViewport(100);
            GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
            GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
        }
    }
}

void AipvmultiCharacter::TogglePauseMenu()
{
    if (!IsLocallyControlled()) return;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || !PauseMenuClass) return;

    bool bIsPaused = PC->IsPaused();
    
    if (bIsPaused)
    {
        if (CurrentPauseWidget)
        {
            CurrentPauseWidget->RemoveFromParent();
            CurrentPauseWidget = nullptr;
        }
        PC->SetPause(false);
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }
    else
    {
        CurrentPauseWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuClass);
        if (CurrentPauseWidget)
        {
            CurrentPauseWidget->AddToViewport(100);
            PC->SetPause(true);
            PC->bShowMouseCursor = true;
            PC->SetInputMode(FInputModeUIOnly());
        }
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
    DOREPLIFETIME(AipvmultiCharacter, ReserveAmmo);
    DOREPLIFETIME(AipvmultiCharacter, MaxReserveAmmo);
}