// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TeamsGameMode.h"

#include "GameState/BlasterGameState.h"
#include "PlayerController/BlasterPlayerController.h"
#include "PlayerState/BlasterPlayerState.h"

ATeamsGameMode::ATeamsGameMode()
{
	bTeamsMatch = true;
}

void ATeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
	if (BGameState)
	{
		ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		if (NewPlayer && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			//按两队人数是否平衡进行分队
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}



void ATeamsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
	if (BGameState)
	{
		for (auto PState: BGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState);
			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
			{
				//按两队人数是否平衡进行分队
				if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
				{
					BGameState->RedTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_RedTeam);
				}
				else
				{
					BGameState->BlueTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_BlueTeam);
				}
			}
		}
	}
}

//计算是否存在友伤
float ATeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	ABlasterPlayerState* AttackerState = Cast<ABlasterPlayerState>(Attacker->GetPlayerState<ABlasterPlayerState>());
	ABlasterPlayerState* VictimState = Cast<ABlasterPlayerState>(Victim->GetPlayerState<ABlasterPlayerState>());
	if (AttackerState == nullptr || VictimState == nullptr) return BaseDamage;

	//自残时
	if (VictimState == AttackerState) return BaseDamage;

	//同队时
	if (AttackerState->GetTeam() == VictimState->GetTeam()) return 0.f;

	return BaseDamage;
}

void ATeamsGameMode::PlayerEliminated(class ABlastCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
	ABlasterPlayerState* AttackPlayerState = AttackerController? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	if (BGameState && AttackPlayerState)
	{
		if (AttackPlayerState->GetTeam() == ETeam::ET_BlueTeam) BGameState->BlueTeamScore;
		if (AttackPlayerState->GetTeam() == ETeam::ET_RedTeam) BGameState->RedTeamScore;
	}
}

void ATeamsGameMode::Logout(AController* Exiting)
{
	ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
	ABlasterPlayerState* BPState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BGameState && BPState)
	{
		if (BGameState->RedTeam.Contains(BPState))
		{
			//从红队移除玩家
			BGameState->RedTeam.Remove(BPState);
		}
		else
		{
			//从蓝队中移除玩家
			BGameState->BlueTeam.Remove(BPState);
		}
	}
}


