// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/PrimaryAssetId.h"
#include "Engine/DataTable.h"
#include "RPGTypes.generated.h"

class URPGSaveGame;
class URPGItem;
/**
 * 
 */

//武器タイプ
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
   Sword,
   Axe,
   GreatSword,
   DualSword
};

//アクターの方向の判定用
UENUM(BlueprintType)
enum class EDirectionType : uint8
{
   Forward,
   Left,
   Right,
   Behind
};

UENUM(BlueprintType)
enum class ERPGStatusType : uint8
{
    HP,
    MP,
    Attack,
    Vitality,
    Agility,
    CharLevel
};

UENUM(BlueprintType)
enum class EEnableCollisionPoint : uint8
{
	Weapon,
	RightHand,
	LeftHand
};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
    AttackReady,
	Attack,
	StandBy
};

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	Easy,
	Normal,
	Hard
};




USTRUCT(BlueprintType)
struct RPGGAME_API FRPGItemSlot
{
	GENERATED_BODY()

	/** Constructor, -1 means an invalid slot */
	FRPGItemSlot()
		: SlotNumber(-1)
	{}

	FRPGItemSlot(const FPrimaryAssetType& InItemType, int32 InSlotNumber)
		: ItemType(InItemType)
		, SlotNumber(InSlotNumber)
	{}

	/** The type of items that can go in this slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FPrimaryAssetType ItemType;

	/** The number of this slot, 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 SlotNumber;

	/** Equality operators */
	bool operator==(const FRPGItemSlot& Other) const
	{
		return ItemType == Other.ItemType && SlotNumber == Other.SlotNumber;
	}
	bool operator!=(const FRPGItemSlot& Other) const
	{
		return !(*this == Other);
	}

	/** Implemented so it can be used in Maps/Sets */
	friend inline uint32 GetTypeHash(const FRPGItemSlot& Key)
	{
		uint32 Hash = 0;

		Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
		Hash = HashCombine(Hash, (uint32)Key.SlotNumber);
		return Hash;
	}

	/** Returns true if slot is valid */
	bool IsValid() const
	{
		return ItemType.IsValid() && SlotNumber >= 0;
	}
};


USTRUCT(BlueprintType)
struct RPGGAME_API FRPGItemData
{
	GENERATED_BODY()

	
	FRPGItemData()
		: ItemCount(1)
		, ItemLevel(1)
	{}

	FRPGItemData(int32 InItemCount, int32 InItemLevel)
		: ItemCount(InItemCount)
		, ItemLevel(InItemLevel)
	{}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemCount;

	/** The level of this item. This level is shared for all instances, can never be below 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemLevel;

	/** Equality operators */
	bool operator==(const FRPGItemData& Other) const
	{
		return ItemCount == Other.ItemCount && ItemLevel == Other.ItemLevel;
	}
	bool operator!=(const FRPGItemData& Other) const
	{
		return !(*this == Other);
	}

	/** Returns true if count is greater than 0 */
	bool IsValid() const
	{
		return ItemCount > 0;
	}

	/** Append an item data, this adds the count and overrides everything else */
	void UpdateItemData(const FRPGItemData& Other, int32 MaxCount, int32 MaxLevel)
	{
		if (MaxCount <= 0)
		{
			MaxCount = MAX_int32;
		}

		if (MaxLevel <= 0)
		{
			MaxLevel = MAX_int32;
		}

		ItemCount = FMath::Clamp(ItemCount + Other.ItemCount, 1, MaxCount);
		ItemLevel = FMath::Clamp(Other.ItemLevel, 1, MaxLevel);
	}
};


USTRUCT(BlueprintType)
struct FRPGStatus : public FTableRowBase
{  
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float HP;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float MP;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float Attack;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float Vitality;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float Agility;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float Exp;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    int32 CharLevel;

    void CustumizeUpdate(ERPGStatusType UpdateType , float Value)
    {
        switch(UpdateType)
        {
            case ERPGStatusType::HP : 
                HP = Value;
                break;
            
            case ERPGStatusType::MP : 
                MP = Value;
                break;
            
            case ERPGStatusType::Attack : 
                Attack = Value;
                break;
            
            case ERPGStatusType::Vitality : 
                Vitality = Value;
                break;
            
            case ERPGStatusType::Agility : 
                Agility = Value;
                break;

            case ERPGStatusType::CharLevel :
                CharLevel = Value;
                break;
            
            default:
                break;
        }

        return;
    }
};

USTRUCT(BlueprintType)
struct FAICombatRule : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = AI)
	int32 AttackArrowNum = 0;

	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category = AI)
	int32 ReadyArrowNum = 0;

};

//戦闘態勢の準備
DECLARE_MULTICAST_DELEGATE(FOnEnemyFight);

//敵の全滅通知
DECLARE_MULTICAST_DELEGATE(FOnAllEnemiesDefeated);

//ステータスの値が変更された場合の通知
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusValueChanged);

//ステータスロード完了通知
DECLARE_DELEGATE(FOnStatusLoaded);
DECLARE_DYNAMIC_DELEGATE(FOnStatusLoadedBP);

//インベントリーロード完了通知
DECLARE_MULTICAST_DELEGATE(FOnInventoryLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryLoadedBP);

//プレイヤースロットの変更通知
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged , FRPGItemSlot , URPGItem*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChangedBP , FRPGItemSlot , ItemSlot ,  URPGItem* , Item);

//インベントリーのアイテム情報の変更
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged , bool , URPGItem*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChangedBP, bool, bAdded, URPGItem*, Item);

//セーブデータロード完了通知
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoaded, URPGSaveGame*);


