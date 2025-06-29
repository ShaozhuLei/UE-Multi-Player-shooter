// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BlasterGameState.h"

#include "Net/UnrealNetwork.h"
#include "PlayerController/BlasterPlayerController.h"
#include "PlayerState/BlasterPlayerState.h"

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
}

void ABlasterGameState::UpdateTopScore(ABlasterPlayerState* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABlasterGameState::BlueTeamScores()
{
	BlueTeamScore++;
	ABlasterPlayerController* PlayerPC = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerPC) PlayerPC->SetHUDBlueTeamScore(BlueTeamScore);
}

void ABlasterGameState::RedTeamScores()
{
	RedTeamScore++;
	ABlasterPlayerController* PlayerPC = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerPC->SetHUDRedTeamScore(RedTeamScore);
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
	ABlasterPlayerController* PlayerPC = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerPC) PlayerPC->SetHUDBlueTeamScore(BlueTeamScore);
}

void ABlasterGameState::OnRep_RedTeamScore()
{
	ABlasterPlayerController* PlayerPC = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerPC) PlayerPC->SetHUDRedTeamScore(RedTeamScore);
}


