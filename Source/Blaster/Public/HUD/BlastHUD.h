// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlastHUD.generated.h"

class UAnnouncement;
class UCharacterOverlay;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairColor;
};

UCLASS()
class BLASTER_API ABlastHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
	
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }

	UPROPERTY(EditAnywhere, Category="Player State")
	TSubclassOf<UCharacterOverlay> CharacterOverlayClass;

	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	UPROPERTY()
	TObjectPtr<UAnnouncement> Announcement;
	
	void AddCharacterOverlay();
	void AddAnnouncement();

protected:

	virtual void BeginPlay() override;
	
	
private:
	FHUDPackage HUDPackage;
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	
};
