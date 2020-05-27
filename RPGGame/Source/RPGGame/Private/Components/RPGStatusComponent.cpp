// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RPGStatusComponent.h"
#include "RPGGameInstance.h"
#include "RPGSaveGame.h"
#include "RPGGameMode.h"
#include "Characters/RPGPlayerCharacter.h"
#include "RPGPlayerControllerBase.h"
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

	ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(Owner->GetController());
	if(PlayerController)
	{
		PlayerController->OnAllEnemiesDefeated.AddUObject(this , &URPGStatusComponent::SaveStatus);
	}
    
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
		if(PlayerSaveData && PlayerSaveData->PlayerStatus.CharLevel > 0) 
		{   
			//データがあれば取得、なければデフォルトで追加したステータスを保存 
			Owner->ParentStatus = PlayerSaveData->PlayerStatus;	
			Owner->CharStatus = Owner->ParentStatus;
			
			if(PlayerSaveData->RemainExp > 0.f)
			{
				Owner->CharStatus.Exp = PlayerSaveData->RemainExp;
				
			}
			
		}
		
	}

	if(OnStatusLoaded.IsBoundToObject(Owner))
	{
		OnStatusLoaded.Execute();	
	}
	
	
	return true;
}

void URPGStatusComponent::SaveStatus()
{
	UWorld* World = GetWorld();
	URPGGameInstance* GameInstance = World ? World->GetGameInstance<URPGGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return;
	}

	URPGSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	ARPGPlayerCharacter* Player = Cast<ARPGPlayerCharacter>(Owner);
	
	if(CurrentSaveGame)
	{   
		CurrentSaveGame->PlayerStatus = Owner->ParentStatus;
		if(Owner->ParentStatus.Exp > Owner->CharStatus.Exp)
		{
			CurrentSaveGame->RemainExp = Owner->CharStatus.Exp;
		
			UE_LOG(LogRPG , Warning , TEXT("Exp over"));
		}


		GameInstance->WriteSaveGame();
		return;
	}


	return;
}

void URPGStatusComponent::AddDefaultStatus(int32 Lv)
{
    FString RowName = (Owner->ActorHasTag("Boss"))? "Boss" : "Lv"+ FString::FromInt(Lv);
	FRPGStatus* Row = DefaultTable->FindRow<FRPGStatus>(*RowName , FString() , true);

	if(Row)
	{   
		Owner->CharStatus = *Row;
		Owner->ParentStatus = *Row;
       
	}
	else
	{
		UE_LOG(LogRPG , Warning , TEXT("Don't find row"));	
	}
}

void URPGStatusComponent::LevelUp(FRPGStatus& Status)
{   
	//UpdateStatus
	if(Status.CharLevel ++ >10)
	{
        Status.HP += 20;
	    Status.MP += 10;
	    Status.Attack += 5;
	    Status.Vitality += 5;
	    Status.Agility += 20;
	    Status.Exp = Status.Exp * 1.2f;
	    Status.CharLevel++;
	}
	else
	{
		FString RowName = "Lv"+ FString::FromInt(Status.CharLevel++);
	    FRPGStatus* Row = DefaultTable->FindRow<FRPGStatus>(*RowName , FString() , true);

		if(Row)
		{
			Status = *Row;
		}
	}
    

	SaveStatus();
}



