// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupSpawnPoint.h"

#include "Pickups/Pickup.h"

// Sets default values
APickupSpawnPoint::APickupSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnPickupTimer(nullptr);
}

void APickupSpawnPoint::SpawnPickup()
{
	int32 NumPickClasses = PickupClasses.Num();
	if (NumPickClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickClasses - 1);
		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform());
		if (HasAuthority() && SpawnedPickup)
		{
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnPoint::StartSpawnPickupTimer);
		}
	}
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
	if (HasAuthority()) SpawnPickup();
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	GetWorldTimerManager().SetTimer(SpawnPickupTimer, this, &APickupSpawnPoint::SpawnPickupTimerFinished, 4);
}

// Called every frame
void APickupSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

