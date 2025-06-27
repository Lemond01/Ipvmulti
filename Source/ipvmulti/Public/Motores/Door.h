// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class IPVMULTI_API ADoor : public AActor
{
	GENERATED_BODY()
    
public:
	ADoor();

	UPROPERTY(EditAnywhere)
	int32 RequiredKeys = 1;

	FTimerHandle WinWidgetTimerHandle;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* SolidCollision;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
					   bool bFromSweep, const FHitResult& SweepResult);

	void OpenDoor();

	void ShowWinWidgetDelayed();
};
