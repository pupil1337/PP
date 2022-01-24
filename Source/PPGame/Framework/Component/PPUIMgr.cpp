// Fill out your copyright notice in the Description page of Project Settings.


#include "PPUIMgr.h"

#include "PPGame/Framework/UI/PPPanelWidget.h"

UPPUIMgr::UPPUIMgr()
{
	CompSpawnCondition = EPPCompSpawnCondition::EPPCSC_OnlyOnAutonomousProxy;
}

void UPPUIMgr::PPInitComponent()
{
	Super::PPInitComponent();

	Panel = Cast<UPPPanelWidget>(CreateWidget(GetWorld(), PanelClass, TEXT("Panel")));
	if (IsValid(Panel))
	{
		Panel->AddToViewport(0);
	}
}

void UPPUIMgr::OnHealth(float NewHealth)
{
	if (IsValid(Panel))
	{
		Panel->OnHealth(NewHealth);
	}
}

void UPPUIMgr::OnWeapon(int CurrAmmo, int SpareAmmo)
{
	if (IsValid(Panel))
	{
		Panel->OnWeapon(CurrAmmo, SpareAmmo);
	}
}
