
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.h"
#include "Components/ActorComponent.h"
#include "HUD/BlastHUD.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	friend class ABlastCharacter;

	void EquipWeapon(class AWeapon* WeaponToEquip);
	void FireButtonPressed(bool bPressed);
	int32 AmountToReload();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);
	void TraceUnderCrosshairs(FHitResult& HitResult);
	void SetHUDCrosshair(float DeltaTime);
	void Fire();
	void Reload();
	void HandleReload();

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(Server, Reliable)
	void ServerReload();

private:

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float DefaultFOV;
	float CurrentFOV;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	bool bFireButtonPressed;
	FVector HitTarget;
	FHUDPackage HUDPackage;
	FTimerHandle FireTimer;
	bool bCanFire = true;
	TMap<EWeaponType, int32> CarriedAmmoMap;
	
	void StartFireTimer();
	void FireTimerFinished();
	void UpdateAmmoValues();
	bool CanFire();

	UPROPERTY()
	TObjectPtr<ABlastCharacter> Character;
	
	UPROPERTY()
	class ABlasterPlayerController* Controller;
	
	UPROPERTY()
	class ABlastHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UPROPERTY(ReplicatedUsing=Onrep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	UFUNCTION()
	void OnRep_CombatState();

	void InterpFOV(float DeltaTime);

	UPROPERTY(EditDefaultsOnly)
	int32 StartingARAmmo = 30;
	
	void InitializeCarriedAmmo();
};


