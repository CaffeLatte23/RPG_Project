// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RPGInventoryComponent.h"
#include "RPGGameInstance.h"
#include "RPGSaveGame.h"
#include "Items/RPGItem.h"

// Sets default values for this component's properties
URPGInventoryComponent::URPGInventoryComponent()
{
	
}


// Called when the game starts
void URPGInventoryComponent::BeginPlay()
{   
    LoadInventory();

	Super::BeginPlay();
}


bool URPGInventoryComponent::AddInventoryItem(URPGItem* NewItem , int32 ItemCount , int32 ItemLevel , bool bAutoSlot)
{
	bool bChanged = false;
    if(!NewItem)
	{
		UE_LOG(LogRPG , Warning , TEXT("AddInventoryItem : Falling trying to add null item!"));
		return false;
	}

	if(ItemCount <= 0 || ItemLevel <= 0)
	{
        UE_LOG(LogRPG , Warning , TEXT("AddInventoryItem : Falling trying to add item negative count or level!"));
		return false;
	}

	FRPGItemData OldData;
	GetInventoryData(NewItem , OldData);

	FRPGItemData NewData = OldData;
	NewData.UpdateItemData(FRPGItemData(ItemCount , ItemLevel) , NewItem->MaxCount , NewItem->MaxLevel);

	if(OldData != NewData)
	{
		InventoryData.Add(NewItem , NewData);
		NotifyInventoryItemChanged(true , NewItem);
		bChanged = true;
	}

	if(bAutoSlot)
	{
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if(bChanged)
	{   
		
		SaveInventory();
		return true;
	}
    
	return false;

}

bool URPGInventoryComponent::RemoveInventoryItem(URPGItem* RemovedItem , int32 RemoveCount)
{
	bool bChanged = false;
	if(!RemovedItem)
	{
		UE_LOG(LogRPG , Warning , TEXT("RemoveInventoryItem : Falling trying to Remove null item!"));
		return false;
	}

	FRPGItemData NewData;
	GetInventoryData(RemovedItem , NewData);

	if(!NewData.IsValid())
	{   
		// wasn't found
		return false;
	}
    
	// If RemoveCount <= 0 , delete all
	if(RemoveCount <= 0)
	{
		NewData.ItemCount = 0;
	}
	else
	{
		NewData.ItemCount -= RemoveCount;
	}

	if(NewData.ItemCount > 0)
	{   
		//Updatedata with new Count
		InventoryData.Add(RemovedItem , NewData);
	}
	else
	{
		InventoryData.Remove(RemovedItem);

		for(TPair<FRPGItemSlot , URPGItem*>& Pair : SlottedItems)
		{
			if(Pair.Value == RemovedItem)
			{   
				Pair.Value = nullptr;
				NotifySlottedItemChanged(Pair.Key , Pair.Value);
			}
		}
	}
	
    NotifyInventoryItemChanged(false , RemovedItem);

	SaveInventory();
    return true;
	
}

bool URPGInventoryComponent::GetInventoryData(URPGItem* Item , FRPGItemData& ItemData) const
{
	const FRPGItemData* FoundItem = InventoryData.Find(Item);

	if(FoundItem)
    {   
		ItemData = *FoundItem;
		return true;
	}
    
	ItemData = FRPGItemData(0,0);
	return false;
}

void URPGInventoryComponent::GetInventoryItems(TArray<URPGItem*>& Items , FPrimaryAssetType ItemType)
{
	for (const TPair<URPGItem* , FRPGItemData>& Pair : InventoryData)
	{
		if(Pair.Key)
		{
			FPrimaryAssetId AssetId = Pair.Key->GetPrimaryAssetId();

			//Filter
			if(AssetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
			{
                Items.Add(Pair.Key);
			}
		}
	}
	
}

int32 URPGInventoryComponent::GetInventoryItemCount(URPGItem* Item) const
{
	const FRPGItemData* FoundItem = InventoryData.Find(Item);

	if(FoundItem)
	{
        return FoundItem->ItemCount;
	}

	return 0;
}

bool URPGInventoryComponent::SetSlottedItem(FRPGItemSlot ItemSlot , URPGItem* Item)
{
	bool bFound = false;
	for( TPair<FRPGItemSlot , URPGItem*>& Pair : SlottedItems )
	{
		if(Pair.Key == ItemSlot)
		{
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key , Pair.Value);
		}
		else if(Item != nullptr && Pair.Value == Item)
		{
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key , Pair.Value);
		}
	}

	if(bFound)
	{
		SaveInventory();
		return true;
	}

	return false;
}

URPGItem* URPGInventoryComponent::GetSlottedItem(FRPGItemSlot ItemSlot) const
{
	URPGItem* const* FoundItem = SlottedItems.Find(ItemSlot);

	if(FoundItem)
	{   
		return *FoundItem;
	}
	return nullptr;
}

void URPGInventoryComponent::GetSlottedItems(TArray<URPGItem*>& Items , FPrimaryAssetType ItemType , bool bOutputEmptyIndexes)
{   
    Items.Reset();

    for(TPair<FRPGItemSlot , URPGItem*>& Pair : SlottedItems)
	{
		if(Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{   
			Items.Add(Pair.Value);
		}
	}
}

void URPGInventoryComponent::FillEmptySlots()
{
	bool bShouldSave = false;
	for(const TPair<URPGItem* , FRPGItemData>& Pair : InventoryData)
	{   
		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
	}

	if(bShouldSave)
	{   
		
		SaveInventory();
	}
}

bool URPGInventoryComponent::FillEmptySlotWithItem(URPGItem* NewItem)
{
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FRPGItemSlot EmptySlot;

	for(TPair<FRPGItemSlot , URPGItem*>& Pair : SlottedItems)
	{
		if(Pair.Key.ItemType == NewItemType)
		{
			if(Pair.Value == NewItem)
			{
				return false;
			}
			else if(Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				EmptySlot = Pair.Key;
			}
		}
	}

	if(EmptySlot.IsValid())
	{
		SlottedItems[EmptySlot] = NewItem;
		NotifySlottedItemChanged(EmptySlot , NewItem);
		return true;
	}

	return false;
}

bool URPGInventoryComponent::LoadInventory()
{
	InventoryData.Reset();
	SlottedItems.Reset();

	

	URPGGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<URPGGameInstance>() : nullptr;

	if(!GameInstance)
	{
		return false;
	}

	if(!GameInstance->OnSaveGameLoaded.IsBoundToObject(this))
	{   
        GameInstance->OnSaveGameLoaded.AddUObject(this , &URPGInventoryComponent::HandleSaveGameLoaded);
	}

	for (const TPair<FPrimaryAssetType , int32>& Pair : GameInstance->ItemSlotsPerType)
	{
		for(int32 SlotNum = 0; SlotNum < Pair.Value ; SlotNum++)
		{
			SlottedItems.Add(FRPGItemSlot(Pair.Key , SlotNum) , nullptr);
		}
	}

	URPGSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	URPGAssetManager& AssetManager = URPGAssetManager::Get();

	if(CurrentSaveGame)
	{
		bool bFoundAnySlots = false;
		for(const TPair<FPrimaryAssetId , FRPGItemData>& ItemPair : CurrentSaveGame->InventoryData)
		{
            URPGItem* LoadedItem = AssetManager.ForceLoadedItem(ItemPair.Key);

			if(LoadedItem != nullptr)
			{
				InventoryData.Add(LoadedItem , ItemPair.Value);
			}
		}

		for(const TPair<FRPGItemSlot , FPrimaryAssetId>& SlotPair : CurrentSaveGame->SlottedItems)
		{
			if(SlotPair.Value.IsValid())
			{
				URPGItem* LoadedItem = AssetManager.ForceLoadedItem(SlotPair.Value);
				if(GameInstance->IsValidItemSlot(SlotPair.Key) && LoadedItem)
				{   
					
                    SlottedItems.Add(SlotPair.Key , LoadedItem);
					bFoundAnySlots = true;
				}
			}
			
		}

		if(!bFoundAnySlots)
		{   
			FillEmptySlots();
		}
        
		NotifyInventoryLoaded();
		return true;
	}

	NotifyInventoryLoaded();
	return false;
	
}

bool URPGInventoryComponent::SaveInventory()
{
	URPGGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<URPGGameInstance>() : nullptr;

	if(!GameInstance)
	{
		return false;
	}

	URPGSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if(CurrentSaveGame)
	{
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for(const TPair<URPGItem* , FRPGItemData>& Pair : InventoryData)
		{
			FPrimaryAssetId AssetId;

			if(Pair.Key)
			{
				AssetId = Pair.Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId , Pair.Value);
			}
		}

		for(const TPair<FRPGItemSlot , URPGItem*>& Pair : SlottedItems)
		{
			FPrimaryAssetId AssetId;
			if(Pair.Value)
			{
				AssetId = Pair.Value->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(Pair.Key , AssetId);
		}

		GameInstance->WriteSaveGame();
		return true;
		
	}
	return false;
}

void URPGInventoryComponent::NotifyInventoryItemChanged(bool bAdded , URPGItem* Item)
{    
	OnInventoryItemChanged.Broadcast(bAdded , Item);
	OnInventoryItemChangedBP.Broadcast(bAdded , Item);
}

void URPGInventoryComponent::NotifySlottedItemChanged(FRPGItemSlot ItemSlot , URPGItem* Item)
{    
	OnSlottedItemChanged.Broadcast(ItemSlot , Item);
	OnSlottedItemChangedBP.Broadcast(ItemSlot , Item);
}
void URPGInventoryComponent::NotifyInventoryLoaded()
{
	OnInventoryLoaded.Broadcast();
	OnInventoryLoadedBP.Broadcast();
}

void URPGInventoryComponent::HandleSaveGameLoaded(URPGSaveGame* NewSaveGame)
{   
	LoadInventory();
}
