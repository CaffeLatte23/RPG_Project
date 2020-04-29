// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAssetManager.h"
#include "Items/RPGItem.h"

const FPrimaryAssetType URPGAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType URPGAssetManager::WeaponItemType = TEXT("Weapon");

URPGAssetManager& URPGAssetManager::Get()
{
    URPGAssetManager* This = Cast<URPGAssetManager>(GEngine->AssetManager);

    if(This)
    {
        return *This;
    }
    else
    {
        UE_LOG(LogRPG, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<URPGAssetManager>(); // never calls this
    }
    
}



URPGItem* URPGAssetManager::ForceLoadedItem(const FPrimaryAssetId& PrimaryAssetId , bool bLogWarning )
{
    FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

    URPGItem* LoadedItem = Cast<URPGItem>(ItemPath.TryLoad());

    if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogRPG, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
