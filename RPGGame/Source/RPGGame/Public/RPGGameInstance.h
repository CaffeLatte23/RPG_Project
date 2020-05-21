// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

class UURPGSaveGame;
class URPGItem;
/**
 * 
 */
UCLASS()
class RPGGAME_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    
	URPGGameInstance();

	virtual void Init() override;

	UPROPERTY()
	class ARPGPlayerCharacter* PlayerCharacter; 
    
	//ゲームモードレベル（レベル間での連絡用)
    UPROPERTY()
	EGameMode GameModeLevel = EGameMode::Normal;

///Inventory//////////
    UPROPERTY(EditAnywhere , Category = "Inventory")
    TMap<FPrimaryAssetId , FRPGItemData> DefaultInventory;

	UPROPERTY(EditAnywhere , Category = "Inventory")
	TMap<FPrimaryAssetType , int32> ItemSlotsPerType;

	UPROPERTY()
	TArray<URPGItem*> StoreItems;

    UFUNCTION()
	void AddDefaultInventory(URPGSaveGame* SaveGame , bool bRemoveExtra = false);

	UFUNCTION()
	void InitializeStoredItems();

///Save///////////////

    FAsyncLoadGameFromSlotDelegate OnLoadCompleted;

	FAsyncSaveGameToSlotDelegate OnSaveCompleted;

	UPROPERTY()
	FString SaveSlot;

	UPROPERTY()
	int32 SaveUserIndex;
    
	//Delegate
	FOnSaveGameLoaded OnSaveGameLoaded;

	UFUNCTION(BlueprintPure , Category = SaveGame)
	URPGSaveGame* GetCurrentSaveGame();
    
	UFUNCTION()
	bool LoadOrCreateSaveGame();

	UFUNCTION()
	bool HandleSaveGameLoaded(USaveGame* SaveGameObject);

	UFUNCTION()
	bool IsValidItemSlot(FRPGItemSlot ItemSlot) const;

	UFUNCTION(BlueprintCallable  , Category = Save)
	bool WriteSaveGame();
    
	UFUNCTION(BlueprintCallable , Category = Save)
	void ResetSaveGame();

	UFUNCTION(BlueprintCallable , Category = Level)
	void RestartLevel();

protected:
    
	UPROPERTY()
	URPGSaveGame* CurrentSaveGame;

	UPROPERTY()
	bool bCurrentlySaving;

	
};
