// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypes.h"
#include "Weapon.generated.h"

class ACasing;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	virtual void Fire(const FVector& HitTarget);
	
	void ShowPickupWidget(bool bShowWidget);
	void SetWeaponState(EWeaponState State);
	void Dropped();
	void AddAmmo(int32 AmmoToAdd);
	void SetHUDAmmo();
	bool IsEmpty() const;
	bool IsFull() const;
	void EnableCustomDepth(bool bEnable);
	FVector TraceEndWithScatter(const FVector& HitTarget);
	

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV(){return ZoomedFOV;}
	FORCEINLINE float GetZoomedInterfSpeed(){return ZoomInterpSpeed;}
	FORCEINLINE EWeaponType GetWeaponType(){return WeaponType;}
	FORCEINLINE int32 GetAmmo(){return Ammo;}
	FORCEINLINE int32 GetMagCapacity(){return MagCapacity;}
	FORCEINLINE float GetDamage(){return Damage;}
	FORCEINLINE float GetheadshotDamage(){return HeadshotDamage;}

	/**
	* Textures for the weapon crosshairs
	*/
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float FireDelay = .15f;

	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	bool bAutomatic = true;

	UPROPERTY(EditAnywhere)
	class USoundBase* EquipSound;

	UPROPERTY(EditAnywhere)
	EFireType FireType;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;

	bool bDestroyWeapon = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();

	UPROPERTY(EditDefaultsOnly, Category="Weapon properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 75.f;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnSphereOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	float HeadshotDamage = 40.f;

	UPROPERTY(Replicated, EditAnywhere)
	bool bUseServerSideRewind = false;

	UPROPERTY()
	class ABlastCharacter* BlasterOwnerCharacter;
	
	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);

private:

	UFUNCTION()
	void SpendRound();

	// The number of unprocessed server requests for Ammo.Add commentMore actions
	// Incremented in SpendRound, decremented in ClientUpdateAmmo.
	int32 Sequence = 0;

	UPROPERTY(VisibleAnywhere, Category="Weapon properties")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UWidgetComponent> PickupWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACasing> CasingSubclass;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	/** 
	* Zoomed FOV while aiming
	*/

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;
	
	UFUNCTION()
	void OnRep_WeaponState();

	UFUNCTION(Client, Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);

	
};




