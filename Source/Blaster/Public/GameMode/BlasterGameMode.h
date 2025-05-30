// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	virtual void PlayerEliminated(class ABlastCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);	
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
};
