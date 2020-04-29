// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "RPGTypes.h"
#include "Components/ActorComponent.h"
#include "RPGInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGGAME_API URPGInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGInventoryComponent();
	virtual void BeginPlay() override;
    
	//所有者の全てのアイテムデータの保存
	UPROPERTY(VisibleAnywhere , Category = Inventory)
	TMap<URPGItem* , FRPGItemData> InventoryData;
    
	//プレイヤースロットの情報変数
	UPROPERTY(VisibleAnywhere , Category = Inventory)
	TMap<FRPGItemSlot , URPGItem*> SlottedItems;
    
	//スロットアイテムの変更通知
	FOnSlottedItemChanged OnSlottedItemChanged;

	UPROPERTY(BlueprintAssignable , Category = Inventory)
	FOnSlottedItemChangedBP OnSlottedItemChangedBP;
    
	//インベントリのアイテム変更通知
	FOnInventoryItemChanged OnInventoryItemChanged;

	UPROPERTY(BlueprintAssignable , Category = Inventory)
	FOnInventoryItemChangedBP OnInventoryItemChangedBP;
    
	//インベントリのロード通知
    FOnInventoryLoaded OnInventoryLoaded;

	UPROPERTY(BlueprintAssignable , Category = Inventory)
	FOnInventoryLoadedBP OnInventoryLoadedBP;

	UFUNCTION(BlueprintImplementableEvent , Category = Inventory)
	void InventoryItemChanged(bool bAdded , URPGItem* Item);
    
	//インベントリへのアイテムの追加
	UFUNCTION(BlueprintCallable , Category = Inventory)
	bool AddInventoryItem(URPGItem* NewItem , int32 ItemCount = 1 , int32 ItemLevel = 1 , bool bAutoSlot = true);
    
    //インベントリへのアイテムの削除
	UFUNCTION(BlueprintCallable , Category = Inventory)
	bool RemoveInventoryItem(URPGItem* RemovedItem , int32 RemoveCount);

	//任意のアイテムの所有情報の取得
	UFUNCTION(BlueprintPure , Category = Inventory)
	bool GetInventoryData(URPGItem* Item , FRPGItemData& ItemData) const;
    
	//タイプのアイテム取得
	UFUNCTION(BlueprintCallable , Category = Inventory)
	void GetInventoryItems(TArray<URPGItem*>& Items , FPrimaryAssetType ItemType);
    
	//任意のアイテムの個数を取得
	UFUNCTION(BlueprintPure , Category = Inventory)
	int32 GetInventoryItemCount(URPGItem* Item) const;

	//スロットへのアイテムセット
	UFUNCTION(BlueprintCallable , Category = Inventory)
	bool SetSlottedItem(FRPGItemSlot ItemSlot , URPGItem* Item);

	//任意のスロットアイテムの取得
	UFUNCTION(BlueprintCallable , Category = Inventory)
	URPGItem* GetSlottedItem (FRPGItemSlot ItemSlot) const;

	//スロットアイテムをすべて取得
	UFUNCTION(BlueprintCallable , Category = Inventory)
	void GetSlottedItems(TArray<URPGItem*>& Items , FPrimaryAssetType ItemType , bool bOutputEmptyIndexes);

	//空白のスロットを埋める
	UFUNCTION(BlueprintCallable , Category = Inventory)
	void FillEmptySlots();	
    
	//空白のスロットをアイテムで埋める
	UFUNCTION(BlueprintCallable , Category = Inventory)
	bool FillEmptySlotWithItem(URPGItem* NewItem);
    
	//インベントリのロード
	UFUNCTION(BlueprintCallable , Category = Inventory)
	bool LoadInventory();
    
	//インベントリのセーブ
	UFUNCTION(BlueprintCallable , Category = Inventory)
	bool SaveInventory();

protected:

    //通知
	void NotifyInventoryItemChanged(bool bAdded , URPGItem* Item);
	void NotifySlottedItemChanged(FRPGItemSlot ItemSlot , URPGItem* Item);
	void NotifyInventoryLoaded();

	void HandleSaveGameLoaded(URPGSaveGame* NewSaveGame);
};
