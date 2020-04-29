// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Engine/AssetManager.h"
#include "RPGAssetManager.generated.h"

class URPGItem;
/**
 * 
 */
UCLASS()
class RPGGAME_API URPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
    
	URPGAssetManager(){}
	
    
	/* Item Type*/
	static const FPrimaryAssetType PotionItemType;
	static const FPrimaryAssetType WeaponItemType;
    
	/* return current asset manager*/
	static URPGAssetManager& Get();


	URPGItem* ForceLoadedItem(const FPrimaryAssetId& PrimaryAssetId , bool bLogWarning = true);
};
