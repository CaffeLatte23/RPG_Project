// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"
#include "RPGAssetManager.h"
#include "Items/RPGItem.h"
#include "Characters/RPGPlayerCharacter.h"
#include "RPGSaveGame.h"

URPGGameInstance::URPGGameInstance()
    :SaveSlot(TEXT("SaveGame"))
    , SaveUserIndex(0)
{}

void URPGGameInstance::Init()
{
    Super::Init();
    
    InitializeStoredItems();

    if(HandleSaveGameLoaded(UGameplayStatics::LoadGameFromSlot(SaveSlot , SaveUserIndex)))
    {
        UE_LOG(LogRPG , Log, TEXT("LoadedSaveGame"));
    }
    else
    {
        WriteSaveGame();
         UE_LOG(LogRPG , Log, TEXT("New GameData Created"));
    }

    PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this , 0));

}

void URPGGameInstance::AddDefaultInventory(URPGSaveGame* SaveGame , bool bRemoveExtra)
{   
    if(bRemoveExtra)
    {
        SaveGame->InventoryData.Reset();
    }

    for(const TPair<FPrimaryAssetId , FRPGItemData>& Pair : DefaultInventory)
    {
        if(!SaveGame->InventoryData.Contains(Pair.Key))
        {   
            SaveGame->InventoryData.Add(Pair.Key , Pair.Value);
        }
    }
}

void URPGGameInstance::InitializeStoredItems()
{   
    TArray<FPrimaryAssetId> LoadList;
    URPGAssetManager& AssetManager = URPGAssetManager::Get();
    
    const TArray<FName> Bundles;
    FStreamableDelegate OnCompleted;

    for(const TPair<FPrimaryAssetType , int32>& Pair : ItemSlotsPerType)
    {   
        LoadList.Reset();
        UKismetSystemLibrary::GetPrimaryAssetIdList(Pair.Key , LoadList);
      
        AssetManager.LoadPrimaryAssets(LoadList , Bundles ,OnCompleted); 

        for(FPrimaryAssetId Id : LoadList)
        {
            URPGItem* Item = Cast<URPGItem>(AssetManager.GetPrimaryAssetObject(Id));

                if(Item)
                {
                    StoreItems.Add(Item);
                }
        } 
    }
    
}

bool URPGGameInstance::LoadOrCreateSaveGame()
{  
    URPGSaveGame* LoadedSave = nullptr;

    if(UGameplayStatics::DoesSaveGameExist(SaveSlot,SaveUserIndex))
    {
        LoadedSave = Cast<URPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot , SaveUserIndex));
    }    
    
    return HandleSaveGameLoaded(LoadedSave);
}

bool URPGGameInstance::IsValidItemSlot(FRPGItemSlot ItemSlot) const
{
    if(ItemSlot.IsValid())
    {
        const int32* FoundCount = ItemSlotsPerType.Find(ItemSlot.ItemType);

        if(FoundCount)
        {
            return ItemSlot.SlotNumber < *FoundCount;
        }
    }

    return false;
}

bool URPGGameInstance::HandleSaveGameLoaded(USaveGame* SaveGameObject)
{   
    bool bLoaded = false;

    CurrentSaveGame = Cast<URPGSaveGame>(SaveGameObject);
    
    if(CurrentSaveGame)
    {
        AddDefaultInventory(CurrentSaveGame , false);
        bLoaded = true;
    }
    else
    {
        CurrentSaveGame = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));
		AddDefaultInventory(CurrentSaveGame, true);
    }
    
    OnSaveGameLoaded.Broadcast(CurrentSaveGame);
    
    return bLoaded;
}

URPGSaveGame* URPGGameInstance::GetCurrentSaveGame()
{
    return CurrentSaveGame;
}

bool URPGGameInstance::WriteSaveGame()
{   
    //Check Save Game Info
    UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame() , SaveSlot , SaveUserIndex);
    return true;
}

void URPGGameInstance::ResetSaveGame()
{
    HandleSaveGameLoaded(nullptr);
}