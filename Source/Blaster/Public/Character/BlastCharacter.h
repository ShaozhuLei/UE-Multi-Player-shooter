// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.h"
#include "InputMappingContext.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "BlastCharacter.generated.h"

class UCombatComponent;
class AWeapon;
class UOverHeadWidget;

UCLASS()
class BLASTER_API ABlastCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlastCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void CrouchButtonPressed();
	void EquipButtonPressed();
	void AimButtonPressed(const FInputActionInstance& Instance);
	void AimButtonReleased();
	void SetOverlappingWeapon(AWeapon* InWeapon);
	void AimOffset();
	bool IsEquipped() const;
	bool IsAiming();
	AWeapon* GetEquippedWeapon();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> SlashMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> OverHeadWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatComponent> Combat;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerSetUseControllerRotationYaw(bool InUse);

	UFUNCTION(Server, Reliable)
	void ServerSetAO_Yaw(float InYaw);
	
	UFUNCTION(Server, Reliable)
	void ServerSetAO_Pitch(float InPitch);

	UFUNCTION(Server, Reliable)
	void ServerSetTurningPlace(ETurningInPlace InTurningInState);
	
	void TurnInPlace();

	UPROPERTY(Replicated)
	ETurningInPlace TurningInPlace;
	
	UPROPERTY(Replicated)
	float AO_Yaw;

	UPROPERTY(Replicated)
	float AO_Pitch;
	
	float InterpAO_Yaw;
	float CachedDeltaTime;
	FRotator StartingAimRotation;
	FRotator EndingAimRotation;
	
};





