// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlastCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BlasterComponents/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"
#include "Blaster/Blaster.h"
#include "BlasterComponents/BuffComponent.h"
#include "GameMode/BlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/BlasterPlayerController.h"
#include "PlayerState/BlasterPlayerState.h"
#include "BlasterComponents/LagCompensationComponent.h"
#include "Components/BoxComponent.h"
#include "BlasterComponents/LagCompensationComponent.h"
#include "GameState/BlasterGameState.h"
// Sets default values
ABlastCharacter::ABlastCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = false;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>("OverheadWidget");
	OverHeadWidget->SetupAttachment(GetRootComponent());

	Combat = CreateDefaultSubobject<UCombatComponent>("Combat");
	Combat->SetIsReplicated(true);

	Buff = CreateDefaultSubobject<UBuffComponent>("BuffComponent");
	Buff->SetIsReplicated(true);

	LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>("LagCompensation");

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>("Grenade Mesh");
	AttachedGrenade->SetupAttachment(GetMesh(), "GrenadeSocket");
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	/** Add commentMore actions
	* Hit boxes for server-side rewind
	*/

	head = CreateDefaultSubobject<UBoxComponent>(TEXT("head"));
	head->SetupAttachment(GetMesh(), FName("head"));
	HitCollisionBoxes.Emplace(FName("Head"), head);

	pelvis = CreateDefaultSubobject<UBoxComponent>(TEXT("pelvis"));
	pelvis->SetupAttachment(GetMesh(), FName("pelvis"));
	HitCollisionBoxes.Emplace(FName("pelvis"), pelvis);

	spine_02 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_02"));
	spine_02->SetupAttachment(GetMesh(), FName("spine_02"));
	HitCollisionBoxes.Emplace(FName("spine_02"), spine_02);

	spine_03 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_03"));
	spine_03->SetupAttachment(GetMesh(), FName("spine_03"));
	HitCollisionBoxes.Emplace(FName("spine_03"), spine_03);

	upperarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("Upperarm_L"));
	upperarm_l->SetupAttachment(GetMesh(), FName("Upperarm_L"));
	HitCollisionBoxes.Emplace(FName("Upperarm_L"), upperarm_l);

	upperarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("Upperarm_R"));
	upperarm_r->SetupAttachment(GetMesh(), FName("Upperarm_R"));
	HitCollisionBoxes.Emplace(FName("Upperarm_R"), upperarm_r);

	lowerarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_l"));
	lowerarm_l->SetupAttachment(GetMesh(), FName("lowerarm_l"));
	HitCollisionBoxes.Emplace(FName("lowerarm_l"), lowerarm_l);

	lowerarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_r"));
	lowerarm_r->SetupAttachment(GetMesh(), FName("lowerarm_r"));
	HitCollisionBoxes.Emplace(FName("lowerarm_l"), lowerarm_l);

	hand_l = CreateDefaultSubobject<UBoxComponent>(TEXT("Hand_L"));
	hand_l->SetupAttachment(GetMesh(), FName("Hand_L"));
	HitCollisionBoxes.Emplace(FName("Hand_L"), hand_l);

	hand_r = CreateDefaultSubobject<UBoxComponent>(TEXT("Hand_R"));
	hand_r->SetupAttachment(GetMesh(), FName("Hand_R"));
	HitCollisionBoxes.Emplace(FName("Hand_R"), hand_r);
	
	thigh_l = CreateDefaultSubobject<UBoxComponent>(TEXT("Thigh_L"));
	thigh_l->SetupAttachment(GetMesh(), FName("Thigh_L"));
	HitCollisionBoxes.Emplace(FName("Thigh_L"), thigh_l);

	thigh_r = CreateDefaultSubobject<UBoxComponent>(TEXT("Thigh_R"));
	thigh_r->SetupAttachment(GetMesh(), FName("Thigh_R"));
	HitCollisionBoxes.Emplace(FName("Thigh_R"), thigh_r);

	calf_l = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_l"));
	calf_l->SetupAttachment(GetMesh(), FName("calf_l"));
	HitCollisionBoxes.Emplace(FName("calf_l"), calf_l);

	calf_r = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_r"));
	calf_r->SetupAttachment(GetMesh(), FName("calf_r"));
	HitCollisionBoxes.Emplace(FName("calf_r"), calf_r);

	foot_l = CreateDefaultSubobject<UBoxComponent>(TEXT("Foot_L"));
	foot_l->SetupAttachment(GetMesh(), FName("Foot_L"));
	HitCollisionBoxes.Emplace(FName("Foot_L"), foot_l);

	foot_r = CreateDefaultSubobject<UBoxComponent>(TEXT("Foot_R"));
	foot_r->SetupAttachment(GetMesh(), FName("Foot_R"));
	HitCollisionBoxes.Emplace(FName("Foot_R"), foot_r);

	for (auto Box : HitCollisionBoxes)
	{
		if (Box.Value)
		{
			Box.Value->SetCollisionObjectType(ECC_HitBox);
			Box.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			Box.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			Box.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
}


// Called when the game starts or when spawned
void ABlastCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const ULocalPlayer* Player = (GEngine && GetWorld())? GEngine->GetFirstGamePlayer(GetWorld()): nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player);
		{
			if (SlashMappingContext) Subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}
	SpawnDefaultWeapon();
	UpdateHUDAmmo();
	UpdateHUDHealth();
	UpdateHUDShield();
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlastCharacter::ReceiveDamage);
	}

	if (AttachedGrenade)
	{
		AttachedGrenade->SetVisibility(false);
	}
	HighlightEnemies();
}

void ABlastCharacter::Destroyed()
{
	Super::Destroyed();
	BlasterGameMode = BlasterGameMode == nullptr? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	bool bMatchNotInProgress = BlasterGameMode && BlasterGameMode->GetMatchState() != MatchState::InProgress;
	if (Combat && Combat->EquippedWeapon && bMatchNotInProgress)
	{
		Combat->EquippedWeapon->Destroy();
	}
}

// Called every frame
void ABlastCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateInPlace(DeltaTime);
	HideCameraIfCharacterClose();
	PollInt();
}

void ABlastCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABlastCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABlastCharacter, Health);
	DOREPLIFETIME(ABlastCharacter, bDisableGameplay);
	DOREPLIFETIME(ABlastCharacter, Shield);
}

void ABlastCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat) Combat->Character = this;
	if (Buff)
	{
		Buff->Character = this;
		Buff->SetInitialSpeeds(
			GetCharacterMovement()->MaxWalkSpeed, 
			GetCharacterMovement()->MaxWalkSpeedCrouched,
			GetCharacterMovement()->JumpZVelocity
		);
	}

	if (LagCompensation)
	{
		LagCompensation->Character = this;
		if (Controller)
		{
			LagCompensation->Controller = Cast<ABlasterPlayerController>(Controller);
		}
	}
	
}

void ABlastCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

void ABlastCharacter::SetOverlappingWeapon(AWeapon* InWeapon)
{
	if (OverlappingWeapon) OverlappingWeapon->ShowPickupWidget(false);
	
	OverlappingWeapon = InWeapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ABlastCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning) InterpAO_Yaw = AO_Yaw;
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir) // running, or jumping
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	CalculateAO_Pitch();
}

void ABlastCharacter::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr && Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABlastCharacter::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void ABlastCharacter::PollInt()
{
	if (BlasterPlayerState == nullptr)
	{
		BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		if (BlasterPlayerState)
		{
			BlasterPlayerState->AddToScore(0.f);
			BlasterPlayerState->AddToDefeats(0);
		}
	}
}

void ABlastCharacter::PlayHitReactMontage()
{
	if (Combat == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) 
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("HitReact1");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABlastCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void ABlastCharacter::SimProxiesTurn()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	bRotateRootBone = false;
	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	UE_LOG(LogTemp, Warning, TEXT("ProxyYaw: %f"), ProxyYaw);

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

}

void ABlastCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	class AController* InstigatorController, AActor* DamageCauser)
{
	BlasterGameMode = BlasterGameMode == nullptr? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	if (bElimmed || BlasterGameMode == nullptr) return;
	Damage = BlasterGameMode->CalculateDamage(InstigatorController, Controller, Damage);
	
	float DamageToHealth = Damage;
	if (Shield > 0.f)
	{
		if (Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
			DamageToHealth = 0.f;
		}
		else
		{
			DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.f, Damage);
			Shield = 0.f;
		}
	}

	Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);
	
	UpdateHUDHealth();
	UpdateHUDShield();
	PlayHitReactMontage();

	if (Health == 0)
	{
		BlasterGameMode = BlasterGameMode == nullptr? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
		if (BlasterGameMode)
		{
			BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
			ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
			BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
		}
	}
}

bool ABlastCharacter::IsEquipped() const
{
	return Combat && Combat->EquippedWeapon;
}

bool ABlastCharacter::IsAiming()
{
	return Combat && Combat->bAiming;
}

void ABlastCharacter::Elim(bool bPlayerLeftGame)
{
	DropOrDestroyWeapons();
	MultiCastElim(bPlayerLeftGame);
	GetWorldTimerManager().SetTimer(ElimTimer, this, &ABlastCharacter::ElimTimerFinished, ElimDelay);
}

void ABlastCharacter::MultiCastElim_Implementation(bool bPlayerLeftGame)
{
	bLeftGame = bPlayerLeftGame;
	if (BlasterPlayerController) BlasterPlayerController->SetHUDWeaponAmmo(0);
	
	bElimmed = true;
	PlayElimMontage();

	//被击杀后 停用移动和输入
	bDisableGameplay = true;
	GetCharacterMovement()->DisableMovement();

	if (Combat) Combat->FireButtonPressed(false);
	
	//停用碰撞
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bool bHideSniperScope = IsLocallyControlled() &&
		Combat && 
		Combat->bAiming && 
		Combat->EquippedWeapon && 
		Combat->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle;
	if (bHideSniperScope)
	{
		ShowSniperScopeWidget(false);
	}
}

void ABlastCharacter::PlayThrowGrenadeMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ThrowGrenadeMontage);
	}
}



void ABlastCharacter::ServerLeaveGame_Implementation()
{
	BlasterGameMode = BlasterGameMode == nullptr? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	BlasterPlayerState = BlasterPlayerState == nullptr? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
	if (BlasterGameMode && BlasterPlayerState) BlasterGameMode->PlayerLeftGame(BlasterPlayerState);
}

void ABlastCharacter::ElimTimerFinished()
{
	BlasterGameMode = BlasterGameMode == nullptr? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	if (BlasterGameMode && !bLeftGame) BlasterGameMode->RequestRespawn(this, Controller);
	if (bLeftGame && IsLocallyControlled()) OnLeftGame.Broadcast();
}

AWeapon* ABlastCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

void ABlastCharacter::Move(const FInputActionValue& InputActionValue)
{
	if (bDisableGameplay) return;
	const FVector2d MovementVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardVector, MovementVector.Y);
	
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightVector, MovementVector.X);
}

void ABlastCharacter::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookDirection = InputActionValue.Get<FVector2D>();
	AddControllerPitchInput(LookDirection.Y);
	AddControllerYawInput(LookDirection.X);
}

void ABlastCharacter::CrouchButtonPressed()
{
	if (bDisableGameplay) return;
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ABlastCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		if (OverlappingWeapon)
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else if (Combat->ShouldSwapWeapons())
		{
			Combat->SwapWeapons();
		}
	}
}

void ABlastCharacter::EquipButtonPressed()
{
	if (bDisableGameplay) return;
	if (Combat)
	{
		ServerEquipButtonPressed();
	}
}

void ABlastCharacter::AimButtonPressed(const FInputActionInstance& Instance)
{
	if (bDisableGameplay) return;
	if (Combat) Combat->SetAiming(true);
}

void ABlastCharacter::AimButtonReleased()
{
	if (bDisableGameplay) return;
	if (Combat) Combat->SetAiming(false);
}

void ABlastCharacter::FireButtonPressed()
{
	if (bDisableGameplay) return;
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void ABlastCharacter::FireButtonReleased()
{
	if (bDisableGameplay) return;
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void ABlastCharacter::ReloadButtonPressed()
{
	if (bDisableGameplay) return;
	if (Combat) Combat->Reload();
}

void ABlastCharacter::GrenadeButtonPressed()
{
	if (Combat) Combat->ThrowGrenade();
}


void ABlastCharacter::OnRep_Health(float LastHealth)
{
	UpdateHUDHealth();
	//此时正在掉血
	if (Health < LastHealth) PlayHitReactMontage();
}

void ABlastCharacter::UpdateHUDHealth()
{
	BlasterPlayerController = BlasterPlayerController == nullptr? Cast<ABlasterPlayerController>(Controller): BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABlastCharacter::UpdateHUDShield()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDShield(Shield, MaxShield);
	}
}

void ABlastCharacter::UpdateHUDAmmo()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if (BlasterPlayerController && Combat && Combat->EquippedWeapon)
	{
		BlasterPlayerController->SetHUDCarriedAmmo(Combat->CarriedAmmo);
		BlasterPlayerController->SetHUDWeaponAmmo(Combat->EquippedWeapon->GetAmmo());
	}
}

void ABlastCharacter::SpawnDefaultWeapon()
{
	BlasterGameMode = BlasterGameMode == nullptr? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	UWorld* World = GetWorld();
	if (BlasterGameMode && World && !bElimmed && DefaultWeaponClass)
	{
		AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClass);
		StartingWeapon->bDestroyWeapon = true;
		if (Combat)
		{
			Combat->EquipWeapon(StartingWeapon);
		}
	}
}

void ABlastCharacter::RotateInPlace(float DeltaTime)
{
	if (bDisableGameplay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
}

void ABlastCharacter::DropOrDestroyWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr) return;
	if (Weapon->bDestroyWeapon)
	{
		Weapon->Destroy();
	}
	else
	{
		Weapon->Dropped();
	}
}

void ABlastCharacter::DropOrDestroyWeapons()
{
	//主武器服务器全部销毁或扔掉
	if (Combat)
	{
		if (Combat->EquippedWeapon)
		{
			DropOrDestroyWeapon(Combat->EquippedWeapon);
		}
		if (Combat->SecondaryWeapon)
		{
			DropOrDestroyWeapon(Combat->SecondaryWeapon);
		}
	}
}

void ABlastCharacter::HighlightEnemies()
{
	BlasterGameMode = BlasterGameMode == nullptr? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	
	ABlasterGameState* BlasterGameState = BlasterGameMode->GetGameState<ABlasterGameState>();
	for (auto PlayerState: BlasterGameState->PlayerArray)
	{
		ABlasterPlayerState* BlasterPlayerState = Cast<ABlasterPlayerState>(PlayerState);
		ABlasterPlayerState* MyPlayerState = Cast<ABlasterPlayerState>(GetPlayerState());
		if (BlasterPlayerState && MyPlayerState)
		{
			if (BlasterPlayerState->GetTeam() != MyPlayerState->GetTeam())
			{
				GetMesh()->SetRenderCustomDepth(true);
				GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
				GetMesh()->MarkRenderStateDirty();
			}
		}
		
	}
}

void ABlastCharacter::PlayReloadMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon ==nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		switch (Combat->EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			SectionName = FName("Rifle");
			break;

		case EWeaponType::EWT_RocketLauncher:
			SectionName = FName("Rifle");
			break;

		case EWeaponType::EWT_Pistol:
			SectionName = FName("Pistol");
			break;

		case EWeaponType::EWT_SubmachineGun:
			SectionName = FName("Rifle");
			break;

		case EWeaponType::EWT_Shotgun:
			SectionName = FName("Shotgun");
			break;

		case EWeaponType::EWT_SniperRifle:
			SectionName = FName("Rifle");
			break;

		case EWeaponType::EWT_GrenadeLauncher:
			SectionName = FName("Rifle");
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}


void ABlastCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon) OverlappingWeapon->ShowPickupWidget(true);

	//若离开可拾取范围，OverlappingWeapon将是nullptr，但是LastWeapon还会指向OverlappingWeapon
	if (LastWeapon) LastWeapon->ShowPickupWidget(false);
}

void ABlastCharacter::HideCameraIfCharacterClose()
{
	if (!IsLocallyControlled()) return;
	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
	{
		GetMesh()->SetVisibility(false);
		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
	}
}

void ABlastCharacter::OnRep_Shield(float LastShield)
{
	UpdateHUDShield();
	if (Shield < LastShield)
	{
		PlayHitReactMontage();
	}
}

void ABlastCharacter::ServerSetTurningPlace_Implementation(ETurningInPlace InTurningInState)
{
	TurningInPlace = InTurningInState;
}

void ABlastCharacter::TurnInPlace(float DeltaTime)
{	
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}


float ABlastCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

bool ABlastCharacter::IsLocallyReloading()
{
	if (Combat == nullptr) return false;
	return Combat->bLocallyReloading;
}


FVector ABlastCharacter::GetHitTarget() const
{
	if (Combat == nullptr) return FVector();

	return Combat->HitTarget;
}

ECombatState ABlastCharacter::GetCombatState() const
{
	if (Combat == nullptr) return ECombatState::ECS_MAX;
	return Combat->CombatState;
}

void ABlastCharacter::SetScopeState(bool ScopeState)
{
	bSniperScopeOn = ScopeState;
}

// Called to bind functionality to input
void ABlastCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlastCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlastCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EPressed, ETriggerEvent::Started, this, &ABlastCharacter::EquipButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABlastCharacter::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &ABlastCharacter::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &ABlastCharacter::AimButtonReleased);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABlastCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABlastCharacter::FireButtonReleased);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABlastCharacter::ReloadButtonPressed);
		EnhancedInputComponent->BindAction(GrenadeAction, ETriggerEvent::Started, this, &ABlastCharacter::GrenadeButtonPressed);
	}
}



