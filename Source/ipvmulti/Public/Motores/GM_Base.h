// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_Base.generated.h"

/**
 * 
 */
UCLASS()
class IPVMULTI_API AGM_Base : public AGameModeBase
{
	GENERATED_BODY()

public:
	int counter = 0;
	
};
