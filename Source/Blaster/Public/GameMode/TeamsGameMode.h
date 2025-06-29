// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/BlasterGameMode.h"
#include "TeamsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ATeamsGameMode : public ABlasterGameMode
{
	GENERATED_BODY()

public:
	ATeamsGameMode();

	virtual void HandleMatchHasStarted() override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(class ABlastCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;

	// 游戏中途加入时触发
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
};
