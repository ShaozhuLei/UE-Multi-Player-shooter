// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BlasterPlayerState.h"

#include "Character/BlastCharacter.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/BlasterPlayerController.h"

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABlasterPlayerState, Defeats);
}


void ABlasterPlayerState::AddToScore(float ScoreAmout)
{
	SetScore(GetScore() + ScoreAmout);
	
	Character = Character == nullptr ? Cast<ABlastCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	Character = Character == nullptr? Cast<ABlastCharacter>(GetPawn()): Character;
	if (Character)
	{
		Controller = Controller == nullptr? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}


void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<ABlastCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void ABlasterPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<ABlastCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}



