// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CollisionBox;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* ImpactSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile Property")
	float Damage;
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
private:
	
	

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* Tracer;

	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;

	
	
};
