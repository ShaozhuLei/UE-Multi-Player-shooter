// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayer = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayer == 1)
	{
		if (GetWorld())
		{
			bUseSeamlessTravel = true;
			GetWorld()->ServerTravel(FString("/Game/Maps/BattleField?listen"));
		}
	}
}
