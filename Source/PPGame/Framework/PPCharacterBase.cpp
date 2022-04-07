// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterBase.h"

#include "Component/PPCompBase.h"

APPCharacterBase::APPCharacterBase()
{
}

void APPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	CreateAllComps(false);
	ENetMode NetMode1 = GetNetMode();
}

void APPCharacterBase::PostNetInit()
{
	CreateAllComps(true);
	
	Super::PostNetInit();
	ENetMode NetMode1 = GetNetMode();
}

void APPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	LinkAndInitAllComps();
	
	if (IsLocallyControlled())
	{
		ChangeControllerRole();
	}
	ENetMode NetMode1 = GetNetMode();
}

void APPCharacterBase::Destroyed()
{
	Super::Destroyed();
}

void APPCharacterBase::CreateAllComps(bool bPostNetInit)
{
	for (auto &it: PPCompClasses)
	{
		CreateComp(bPostNetInit, it);
	}
}

void APPCharacterBase::CreateComp(bool bPostNetInit, TSubclassOf<UPPCompBase> ClassType)
{
	if (IsValid(ClassType))
	{
		UClass* tClass = ClassType.Get();
		if (IsValid(tClass))
		{
			UPPCompBase* tCompDefaultObject = Cast<UPPCompBase>(tClass->GetDefaultObject());
			if (IsValid(tCompDefaultObject))
			{
				bool bNeedCreate = false;
				bool bReplicated = false;
				CreateFilter(bNeedCreate, bReplicated, bPostNetInit, tCompDefaultObject->CompSpawnCondition);
				if (bNeedCreate)
				{
					UPPCompBase* tComp = NewObject<UPPCompBase>(this, tClass);
					check(tComp);
					tComp->RegisterComponent();
					tComp->SetIsReplicated(bReplicated);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Cant Create CompDefaultObject %s"), *tClass->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ClassType Not have UClass???"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ClassType %s is NULL???"));
	}
	
}   	

void APPCharacterBase::CreateFilter(bool& bNeedCreate, bool& bReplicated, bool bPostNetInit, EPPCompSpawnCondition ESpawnCondition)
{
	if (bPostNetInit)
	{
		// PostNetInit(Only RemoteClient)
		if (ESpawnCondition == EPPCompSpawnCondition::EPPCSC_OnlyOnAutonomousProxy && GetLocalRole() == ROLE_AutonomousProxy)
		{
			bNeedCreate = true;
		}
		if (ESpawnCondition == EPPCompSpawnCondition::EPPCSC_OnlyOnSimulatedProxy && GetLocalRole() == ROLE_SimulatedProxy)
		{
			bNeedCreate = true;
		}
	}
	else
	{
		// PostInitializeComponents(Server/Client)
		if (ESpawnCondition == EPPCompSpawnCondition::EPPCSC_Multi && GetLocalRole() == ROLE_Authority)
		{
			bNeedCreate = true;
			bReplicated = true;
		}
		if (ESpawnCondition == EPPCompSpawnCondition::EPPCSC_OnlyOnClients && GetNetMode() != NM_DedicatedServer)
		{
			bNeedCreate = true;
		}
		if (ESpawnCondition == EPPCompSpawnCondition::EPPCSC_OnlyOnServer && GetLocalRole() == ROLE_Authority)
		{
			bNeedCreate = true;
		}
		
		if (ESpawnCondition == EPPCompSpawnCondition::EPPCSC_OnlyOnAutonomousProxy && GetNetMode() == NM_Standalone)
		{
			bNeedCreate = true;
		}
	}
}

void APPCharacterBase::LinkAndInitAllComps()
{
	if (bInitComps)
	{
		return;
	}

	for (auto& it: PPCompClasses)
	{
		LinkAndInitComp(it);
	}

	bInitComps = true;
}

void APPCharacterBase::LinkAndInitComp(TSubclassOf<UPPCompBase> ClassType)
{
	if (IsValid(ClassType))
	{
		UClass* tClass = ClassType.Get();
		if (IsValid(tClass))
		{
			UPPCompBase* tComp = Cast<UPPCompBase>(GetComponentByClass(tClass));
			if (IsValid(tComp))
			{
				PPComps.Add(tComp);
				tComp->PPInitComponent();
			}
			else
			{	
				bool bNeedCreate = false;
				
				UPPCompBase* tCompDefaultObject = Cast<UPPCompBase>(tClass->GetDefaultObject());
				if (IsValid(tCompDefaultObject))
				{
					switch (tCompDefaultObject->CompSpawnCondition)
					{
					case EPPCompSpawnCondition::EPPCSC_Multi:
						{
							bNeedCreate = true;
							break;
						}
					case EPPCompSpawnCondition::EPPCSC_OnlyOnClients:
						{
							if (GetNetMode() != NM_DedicatedServer)
							{
								bNeedCreate = true;
							}
							break;
						}
					case EPPCompSpawnCondition::EPPCSC_OnlyOnServer:
						{
							if (GetLocalRole() == ROLE_Authority)
							{
								bNeedCreate = true;
							}
							break;
						}
					case EPPCompSpawnCondition::EPPCSC_OnlyOnAutonomousProxy:
						{
							if (GetLocalRole() == ROLE_AutonomousProxy || GetNetMode() == NM_Standalone)
							{
								bNeedCreate = true;
							}
							break;
						}
					case EPPCompSpawnCondition::EPPCSC_OnlyOnSimulatedProxy:
						{
							if (GetLocalRole() == ROLE_SimulatedProxy)
							{
								bNeedCreate = true;
							}
							break;
						}
					default:
						bNeedCreate = true;
						break;
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Cant Create CompDefaultObject %s"), *tClass->GetName());
				}
				
				if (bNeedCreate)
				{
					UE_LOG(LogTemp, Error, TEXT("This PPComponent Not Create: LocalRole[ %d ], SpawnCondition[ %d ]"), (int)GetLocalRole(), (int)tCompDefaultObject->CompSpawnCondition);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ClassType Not have UClass???"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ClassType %s is NULL???"));
	}
}

void APPCharacterBase::ChangeControllerRole()
{
	for (auto& it: PPComps)
	{
		if (IsValid(it))
		{
			it->ChangeControllerRole();
		}
	}
}
