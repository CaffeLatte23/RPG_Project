// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "RPGTypes.h"
#include "GameFramework/SaveGame.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class RPGGAME_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	FRPGStatus PlayerStatus;

	FRPGStatus SkillPointRegister;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FPrimaryAssetId, FRPGItemData> InventoryData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FRPGItemSlot, FPrimaryAssetId> SlottedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FString UserId;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	int32 FloorNum = 1;
};
