#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayer.generated.h"

class UBoxComponent;

UCLASS()
class IPVMULTI_API AMyPlayer : public AActor
{
	GENERATED_BODY()
    
public:
	AMyPlayer();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float Health = 100.0f;

	UFUNCTION()
	void ReceiveDamage(float DamageAmount);

protected:
	virtual void BeginPlay() override;
};
