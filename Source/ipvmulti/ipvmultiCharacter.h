// Copyright 1998-2025 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ipvmultiCharacter.generated.h"

UCLASS(config=Game)
class IPVMULTI_API AipvmultiCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AipvmultiCharacter();

	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Base turn rate, in deg/sec */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// --- Salto ---
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Jump() override;  // Sobrescribe la función de ACharacter

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopJumping() override;

	// --- Disparo ---
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	UFUNCTION(Server, Reliable)
	void HandleFire();

	/** Handler for touch input start */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for touch input stop */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	/** Max health of the player */
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** Current health of the player */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	/** RepNotify for changes made to CurrentHealth */
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** Called when health is updated */
	void OnHealthUpdate();

public:
	/** Getter for MaxHealth */
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** Getter for CurrentHealth */
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** Setter for CurrentHealth */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** Handle damage */
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	
protected:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AThirdPersonMPProjectile> ProjectileClass;

	/** Fire rate delay */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	/** Is firing weapon flag */
	bool bIsFiringWeapon;

	/** Timer handle for firing rate */
	FTimerHandle FiringTimer;
};
