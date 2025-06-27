// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ipvmultiCharacter.generated.h"

class UAmmoWidget;
class UMainMenuWidget;
class UPauseMenuWidget;
class UWinWidget;
class UKeyWidget;
class UKeyMessageWidget;

UCLASS(config=Game)
class IPVMULTI_API AipvmultiCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AipvmultiCharacter();

    /** Camera boom */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    /** Health system */
    UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Health")
    float MaxHealth = 100.f;

    /** Ammo system */
    UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmo, BlueprintReadOnly, Category = "Ammo")
    int32 CurrentAmmo;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ammo")
    int32 MaxAmmo = 5;

    UPROPERTY(ReplicatedUsing = OnRep_ReserveAmmo, BlueprintReadOnly, Category = "Ammo")
    int32 ReserveAmmo = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ammo")
    int32 MaxReserveAmmo = 30;

    /** Widget classes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UAmmoWidget> AmmoWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UPauseMenuWidget> PauseMenuClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UWinWidget> WinWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UKeyWidget> KeyWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UKeyMessageWidget> KeyMessageWidgetClass;

    /** Health functions */
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetCurrentHealth(float healthValue);

    /** Ammo functions */
    UFUNCTION(BlueprintPure, Category = "Ammo")
    int32 GetCurrentAmmo() const { return CurrentAmmo; }

    UFUNCTION(BlueprintPure, Category = "Ammo")
    int32 GetMaxAmmo() const { return MaxAmmo; }

    UFUNCTION(BlueprintPure, Category = "Ammo")
    int32 GetReserveAmmo() const { return ReserveAmmo; }

    UFUNCTION(BlueprintPure, Category = "Ammo")
    int32 GetMaxReserveAmmo() const { return MaxReserveAmmo; }

    UFUNCTION(BlueprintCallable, Category = "Ammo")
    void AddAmmoToReserve(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Ammo")
    void UseAmmo(int32 Amount = 1);

    UFUNCTION(BlueprintCallable, Category = "Ammo")
    void Reload();

    virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, 
                           AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void RespawnPlayer();

    /** Shooting system */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Projectile")
    TSubclassOf<class AThirdPersonMPProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    float FireRate = 0.25f;

    UPROPERTY(Replicated)
    bool bIsFiringWeapon = false;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StopFire();

    /** Keys system */
    UPROPERTY(ReplicatedUsing = OnRep_CurrentKeys, BlueprintReadOnly)
    int32 CurrentKeys = 0;

    UFUNCTION(BlueprintCallable, Category = "Keys")
    void AddKey() { CurrentKeys++; OnRep_CurrentKeys(); }

    UFUNCTION(BlueprintCallable, Category = "Keys")
    void UseKeys(int32 Amount) { CurrentKeys = FMath::Max(0, CurrentKeys - Amount); OnRep_CurrentKeys(); }

    UFUNCTION(BlueprintPure, Category = "Keys")
    int32 GetCurrentKeys() const { return CurrentKeys; }

    /** UI Functions */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMainMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void TogglePauseMenu();

protected:
    /** Base turn rate */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
    float TurnRateGamepad;

    FTimerHandle FiringTimer;

    /** Widget instances */
    UPROPERTY()
    UAmmoWidget* AmmoWidgetInstance;

    UPROPERTY()
    UKeyWidget* KeyWidgetInstance;

    UPROPERTY()
    UPauseMenuWidget* CurrentPauseWidget;

    /** Movement functions */
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    /** Touch input */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
    void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

    /** Health replication */
    UFUNCTION()
    void OnRep_CurrentHealth();
    
    UFUNCTION(BlueprintNativeEvent, Category = "Health")
    void OnHealthUpdate();

    /** Ammo replication */
    UFUNCTION()
    void OnRep_CurrentAmmo();

    UFUNCTION()
    void OnRep_ReserveAmmo();

    UFUNCTION(BlueprintCallable, Category = "Ammo")
    void UpdateAmmoWidget();

    /** Keys replication */
    UFUNCTION()
    void OnRep_CurrentKeys();

    /** Shooting implementation */
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void HandleFire();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_HandleFire();

    /** Input */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void BeginPlay() override;

    /** Replication */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};