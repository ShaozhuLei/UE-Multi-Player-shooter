// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Casing.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ACasing::ACasing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;
}

// Called when the game starts or when spawned
void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	
	float EjectionPitch = GetActorRotation().Pitch + FMath::RandRange(-45.f, 45.f);
	FRotator EjectionRotator = FRotator(EjectionPitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	FVector EjectionVector = EjectionRotator.Vector();
	CasingMesh->AddImpulse(EjectionVector * ShellEjectionImpulse);
	
	//CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound) UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	Destroy();
}


