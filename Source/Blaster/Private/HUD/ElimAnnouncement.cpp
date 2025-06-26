// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ElimAnnouncement.h"

void UElimAnnouncement::SetElimAnnouncement(FString AttackerName, FString VictimName)
{
	FString ElimAnnouncementText  = FString::Printf(TEXT("%s Killed %s"), *AttackerName, *VictimName);
	if (AnnouncementText) AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
}
