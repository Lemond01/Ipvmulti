#include "ipvmulti/Public/MyPlayer.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Constructor
AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Crear el componente de colisión
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionComp->SetCollisionProfileName("Pawn");
	CollisionComp->SetNotifyRigidBodyCollision(true); // Para OnHit
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->BodyInstance.SetCollisionProfileName("BlockAll");

	RootComponent = CollisionComp;

	// Enlazar evento de colisión
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyPlayer::OnHit);
}

// Comienza el juego
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Se llama cada frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Maneja el daño recibido
void AMyPlayer::ReceiveDamage(float DamageAmount)
{
	Health -= DamageAmount;

	FString HealthString = FString::Printf(TEXT("Aguante del Muro restante: %.1f"), Health);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, HealthString);

	if (Health <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡Muro Destruido!"));
		Destroy();
	}
}

// Se llama al colisionar
void AMyPlayer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, FVector NormalImpulse,
					  const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("¡Impacto!"));
		ReceiveDamage(35.0f);
		
	}
}




