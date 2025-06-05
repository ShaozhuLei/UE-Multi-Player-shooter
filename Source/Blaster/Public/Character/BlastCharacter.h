// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Interface/InteractWithCrosshairsInterface.h"
#include "BlastCharacter.generated.h"

class UCombatComponent;
class AWeapon;
class UOverHeadWidget;

UCLASS()
class BLASTER_API ABlastCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlastCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void OnRep_ReplicatedMovement() override;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void CrouchButtonPressed();
	void EquipButtonPressed();
	void AimButtonPressed(const FInputActionInstance& Instance);
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void ReloadButtonPressed();
	void SetOverlappingWeapon(AWeapon* InWeapon);
	void AimOffset(float DeltaTime);
	void TurnInPlace(float DeltaTime);
	void PlayFireMontage(bool bAiming);
	void PlayElimMontage();
	void PlayReloadMontage();
	void PollInt();
	bool IsEquipped() const;
	bool IsAiming();
	void Elim();

	UFUNCTION(Netmulticast, Reliable)
	void MultiCastElim();
	
	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const {return bElimmed;}
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }

	/*Movement*/
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> EPressed;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> AimingAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

protected:
	// Called when the game starts or when spawned
	
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void PlayHitReactMontage();
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	void UpdateHUDHealth();
	void RotateInPlace(float DeltaTime);
	

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* ElimMontage;
	
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

private:
	
	ETurningInPlace TurningInPlace;
	float AO_Yaw;
	float AO_Pitch;
	float InterpAO_Yaw;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float TurnThreshold = 0.5f;
	bool bRotateRootBone;
	bool bElimmed = false;
	FRotator StartingAimRotation;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	FTimerHandle ElimTimer;

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	void ElimTimerFinished();
	float CalculateSpeed();

	/** Montages*/
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> SlashMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> OverHeadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> Combat;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category="Player State")
	float Health = 100.f;
	
	UFUNCTION()
	void OnRep_Health();
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	UFUNCTION(Server, Reliable)
	void ServerSetTurningPlace(ETurningInPlace InTurningInState);

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;
};









