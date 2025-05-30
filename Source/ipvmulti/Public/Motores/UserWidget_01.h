
#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ipvmulti/Public/Motores/GM_Base.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_01.generated.h"

class UTextBlock;
class UButton;
class AGM_Base;

UCLASS()
class IPVMULTI_API UUserWidget_01 : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* scoreLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* scoreButon;

	void UpdateScore();

	UFUNCTION()
	void RaiseScore();

	UPROPERTY()
	AGM_Base* myGameMode;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; 

protected:
	virtual void NativeConstruct() override; 
};
