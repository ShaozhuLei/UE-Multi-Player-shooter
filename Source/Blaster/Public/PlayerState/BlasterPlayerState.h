// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.h"
#include "GameFramework/PlayerState.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FORCEINLINE ETeam GetTeam(){return Team;}
	FORCEINLINE void SetTeam(const ETeam team){Team = team;}
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Score() override;
	
	void AddToScore(float ScoreAmout);
	void AddToDefeats(int32 DefeatsAmount);

	UFUNCTION()
	virtual void OnRep_Defeats();

private:
	UPROPERTY()
	class ABlastCharacter* Character;

	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	UPROPERTY(Replicated)
	ETeam Team = ETeam::ET_NoTeam;
};
