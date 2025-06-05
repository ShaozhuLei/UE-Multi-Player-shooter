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
	bool bIsEmpty() const;
	

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV(){return ZoomedFOV;}
	FORCEINLINE float GetZoomedInterfSpeed(){return ZoomInterpSpeed;}
	FORCEINLINE EWeaponType GetWeaponType(){return WeaponType;}
	FORCEINLINE int32 GetAmmo(){return Ammo;}
	FORCEINLINE int32 GetMagCapacity(){return MagCapacity;}

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

private:

	UFUNCTION()
	void SpendRound();
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category="Weapon properties")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UWidgetComponent> PickupWidget;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACasing> CasingSubclass;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	EWeaponType WeaponType;
	
	UPROPERTY()
	class ABlastCharacter* BlasterOwnerCharacter;
	
	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

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

	UFUNCTION()
	void OnRep_Ammo();
};


