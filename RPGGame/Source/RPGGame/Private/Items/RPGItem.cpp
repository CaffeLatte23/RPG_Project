// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RPGItem.h"


bool URPGItem::IsConsumable() const
{
    if(MaxCount <= 0)
    {
        return true;
    }

    return false;
}

FString URPGItem::GetIdentifierString() const
{
    return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId URPGItem::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(ItemType , GetFName());
}