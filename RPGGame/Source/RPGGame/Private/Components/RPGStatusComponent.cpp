// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RPGStatusComponent.h"
#include "RPGGameInstance.h"
#include "RPGSaveGame.h"
#include "RPGGameMode.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Engine/DataTable.h"


// Sets default values for this component's properties
URPGStatusComponent::URPGStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFile(TEXT("DataTable'/Game/RPGGame/DataAsset/DT_StatusTable.DT_StatusTable'"));
    if(DataTableFile.Object){
        DefaultTable = DataTableFile.Object;
    }

	// ...
}


// Called when the game starts
void URPGStatusComponent::BeginPlay()
{   
	LoadStatus();
	Super::BeginPlay();	
}

bool URPGStatusComponent::LoadStatus()
{
	UWorld* World = GetWorld();
	URPGGameInstance* GameInstance = World ? World->GetGameInstance<URPGGameInstance>() : nullptr;


	if(!GameInstance || !DefaultTable)
	{   
		return false;
	}
    
	URPGSaveGame* PlayerSaveData = GameInstance->GetCurrentSaveGame();
	Owner = Cast<ARPGCharacterBase>(this->GetOwner());
	AddDefaultStatus(PlayerSaveData->FloorNum);
	
	
	//オーナーがプレイヤーならセーブデータからステータスを取得
    if(Cast<ARPGPlayerCharacter>(Owner))
	{   

		if(PlayerSaveData) 
		{   
			//データがあれば取得、なければデフォルトで追加したステータスを保存
			if( PlayerSaveData->PlayerStatus.CharLevel > 1)
		    {   
			    OwnerStatus = PlayerSaveData->PlayerStatus;
			    return true;
			}
          
			PlayerSaveData->PlayerStatus = OwnerStatus;
		}
		
	}
	
	
	return true;
}

bool URPGStatusComponent::SaveStatus()
{
	UWorld* World = GetWorld();
	URPGGameInstance* GameInstance = World ? World->GetGameInstance<URPGGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	URPGSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if(CurrentSaveGame)
	{   
		CurrentSaveGame->PlayerStatus = OwnerStatus;

		GameInstance->WriteSaveGame();
		return true;
	}

	return false;
}

void URPGStatusComponent::UpdateStatus()
{
	
}

void URPGStatusComponent::AddDefaultStatus(int32 Lv)
{
    FString RowName = (Owner->ActorHasTag("Boss"))? "Boss" : "Lv"+ FString::FromInt(Lv);
	FRPGStatus* Row = DefaultTable->FindRow<FRPGStatus>(*RowName , FString() , true);

	if(Row->HP > 0)
	{   
		OwnerStatus = *Row;
        Owner->CharStatus = *Row;	

		SaveStatus();
		
		if(OnStatusLoaded.IsBoundToObject(Owner))
		{
			OnStatusLoaded.Execute();	
		}
       
	}
	else
	{
		UE_LOG(LogRPG , Warning , TEXT("Don't find row"));	
	}
}

FRPGStatus URPGStatusComponent::LevelUp()
{   
	//UpdateStatus
    OwnerStatus.HP += 20;
	OwnerStatus.MP += 10;
	OwnerStatus.Attack += 5;
	OwnerStatus.Vitality += 5;
	OwnerStatus.Agility += 20;
	OwnerStatus.Exp = OwnerStatus.Exp * 1.2f;
	OwnerStatus.CharLevel++;

	SaveStatus();

	ARPGPlayerCharacter* Player = Cast<ARPGPlayerCharacter>(Owner);
	if(Player && Player->OnLevelUp.IsBound())
	{
		Player->OnLevelUp.Broadcast(OwnerStatus.CharLevel);
	}
    
	return OwnerStatus;
}



