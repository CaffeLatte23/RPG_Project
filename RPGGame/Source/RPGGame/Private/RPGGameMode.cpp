// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameMode.h"
#include "RPGGameInstance.h"
#include "RPGSaveGame.h"
#include "Blueprint/UserWidget.h"
#include "RPGPlayerControllerBase.h"
#include "Engine/DataTable.h"
#include "Characters/RPGPlayerCharacter.h"


ARPGGameMode::ARPGGameMode()
{
    //DefaultPawnClass = ARPGPlayerCharacter::StaticClass();
    //PlayerControllerClass = ARPGPlayerControllerBase::StaticClass();
    
    //ゲームルールが記述されているCSVファイルの取得
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFile(TEXT("DataTable'/Game/RPGGame/DataAsset/DT_AICombatNumRule.DT_AICombatNumRule'"));
    if(DataTableFile.Object)
    {
        CombatRuleTable = DataTableFile.Object;
    }
}

void ARPGGameMode::BeginPlay()
{   
    Super::BeginPlay();

    URPGGameInstance* GameInstance = GetWorld()->GetGameInstance<URPGGameInstance>();
    if(GameInstance)
    {
        SaveGame = GameInstance->GetCurrentSaveGame();
        if(SaveGame)
        {
            GetGameModeRule(SaveGame->GameLevel);
        }
        
    }
    
    ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(UGameplayStatics::GetPlayerController(this , 0));
    if(PlayerController)
    {
        PlayerController->OnAllEnemiesDefeated.AddUObject(this , &ARPGGameMode::ChangeMusic , false);
    }
    if(SaveGame)
    {
        FloorNum = (SaveGame->FloorNum == 0) ? 1 : SaveGame->FloorNum;
    }
}

void ARPGGameMode::GetGameModeRule(EGameMode GameLevel)
{
    FString RowName = "";
    UE_LOG(LogRPG,Error , TEXT("Row not find"));

    switch (GameLevel)
    {
    case EGameMode::Easy :
        RowName = "Easy";
        break;
    case EGameMode::Normal : 
        RowName = "Normal";
        break;
    case EGameMode::Hard :
        RowName = "Hard";
        break; 
    default:
        RowName = "Normal";
        break;
    }
    
    FAICombatRule* Row =  CombatRuleTable->FindRow<FAICombatRule>(*RowName , FString());
    if(Row != nullptr)
    {
        CombatNumRule = *Row;
        UE_LOG(LogRPG,Error , TEXT("Row find : %d"),CombatNumRule.AttackArrowNum);
    }
    else
    {
        UE_LOG(LogRPG,Error , TEXT("Row not find"));
    }

    

}

void ARPGGameMode::MoveNextFloor()
{   
    URPGGameInstance* GameInstance = GetWorld()->GetGameInstance<URPGGameInstance>();
    

    if(!GameInstance)
    {
        return;
    }
    
    if(SaveGame)
    {
        SaveGame->FloorNum += 1;
        UGameplayStatics::AsyncSaveGameToSlot(SaveGame , GameInstance->SaveSlot , GameInstance->SaveUserIndex , FAsyncSaveGameToSlotDelegate::CreateUObject(this , &ARPGGameMode::MoveToLevel));
    }
    else
    {
        JumpToFloor("DungeonLevel");
    }
    
}

void ARPGGameMode::MoveToLevel(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{   
    JumpToFloor("DungeonLevel");
}

void ARPGGameMode::JumpToFloor(FName Name)
{
    ARPGPlayerCharacter* Player = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if(Player)
    {
        GetWorldTimerManager().PauseTimer(Player->SearchHandle);
    }

    UGameplayStatics::OpenLevel(GetWorld() , Name);
}
