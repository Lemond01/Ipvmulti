// Copyright 1998-2025 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ipvmultiCharacter.generated.h"

UCLASS(config=Game)
class IPVMULTI_API AipvmultiCharacter : public ACharacter
{
    GENERATED_BODY()

    /** Camera boom */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

public:
    AipvmultiCharacter();

    /** Base turn rate */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
    float TurnRateGamepad;

protected:
    /** Movement functions */
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    /** Touch input */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
    void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

    /** Health system */
    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float MaxHealth = 100.f;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
    float CurrentHealth;

    UFUNCTION()
    void OnRep_CurrentHealth();

    void OnHealthUpdate();

public:
    UFUNCTION(BlueprintCallable)
    void SetCurrentHealth(float healthValue);

    virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, 
                           AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable)
    void RespawnPlayer();

    /** Shooting */
    UFUNCTION(BlueprintCallable)
    void StartFire();

    UFUNCTION(BlueprintCallable)
    void StopFire();

    UFUNCTION(Server, Reliable)
    void HandleFire();

    /** Input */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Keys system */
    UPROPERTY(Replicated)
    int32 CurrentKeys = 0;

    UFUNCTION(BlueprintCallable)
    void AddKey() { CurrentKeys++; }

    UFUNCTION(BlueprintCallable)
    void UseKeys(int32 Amount) { CurrentKeys = FMath::Max(0, CurrentKeys - Amount); }

    UFUNCTION(BlueprintPure)
    int32 GetCurrentKeys() const { return CurrentKeys; }

    /** Replication */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    /** Projectile class */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
    TSubclassOf<class AThirdPersonMPProjectile> ProjectileClass;

    /** Fire rate */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
    float FireRate;

    /** Firing flag */
    bool bIsFiringWeapon;

    /** Timer */
    FTimerHandle FiringTimer;
};