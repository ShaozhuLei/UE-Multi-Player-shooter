// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUD/BlastHUD.h"
#include "BlasterPlayerController.generated.h"

class ABlasterPlayerState;
class ABlastCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 DefeatsAmount);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float Countdown);
	void SetHUDGrenades(int32 Grenades);
	void OnMatchStateSet(FName State, bool bTeamsMatch = false);
	void HandleMatchHasStarted(bool bTeamsMatch = false);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void HandleCooldown();
	void HighPingWarning();
	void StopHighPingWarning();
	void CheckPing(float DeltaTime);
	void BroadcastElim(APlayerState* Attacker, APlayerState* Victim);
	void HideTeamScores();
	void InitTeamScores();
	void SetHUDRedTeamScore(int32 RedScore);
	void SetHUDBlueTeamScore(int32 BlueScore);
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float GetServerTime(); // Synced with server world clockAdd commentMore actions
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

	float SingleTripTime = 0.f;

	FHighPingDelegate HighPingDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	void SetHUDTime();
	void PollInit();
	void CheckTimeSync(float DeltaTime);
	void ShowReturnToMainMenu();

	UFUNCTION(Client, Reliable)
	void ClientElimAnnouncement(APlayerState* Attacker, APlayerState* Victim);

	// Requests the current server time, passing in the client's time when the request was sentAdd commentMore actions
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	float ClientServerDelta = 0.f; // difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_ShowTeamScores)
	bool bShowTeamScores = false;

	UFUNCTION()
	void OnRep_ShowTeamScores();

	
	FString GetInfoText(TArray<ABlasterPlayerState*>& Players);
	FString GetTeamInfoText(class ABlasterGameState* BlasterGameState);

private:

	UPROPERTY()
	ABlastHUD* BlasterHUD;

	//匹配总用时
	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
	class UReturnToMainMenu* ReturnToMainMenu;
	
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;
	
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;
	
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;
	
	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);

	bool bReturnToMainMenuOpen = false;
	uint32 CountdownInt = 0;
	float HighPingRunningTime = 0.f;
	float PingAnimationRunningTime = 0.f;
	float HUDHealth;
	bool bInitializeHealth = false;
	float HUDMaxHealth;
	float HUDScore;
	bool bInitializeScore = false;
	int32 HUDDefeats;
	bool bInitializeDefeats = false;
	int32 HUDGrenades;
	bool bInitializeGrenades = false;
	float HUDShield;
	bool bInitializeShield = false;
	float HUDMaxShield;
	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;
	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;
};








