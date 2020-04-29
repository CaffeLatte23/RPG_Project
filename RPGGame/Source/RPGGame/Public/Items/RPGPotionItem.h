// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Items/RPGItem.h"
#include "RPGPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API URPGPotionItem : public URPGItem
{
	GENERATED_BODY()

public:
    
	URPGPotionItem()
	{
	    ItemType = URPGAssetManager::PotionItemType;
	}

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Item)
	ERPGStatusType EffectType = ERPGStatusType::HP;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Item)
	float EffectValue = 0.f;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Item)
	float EffectTime = 0.f;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Item)
	UAnimMontage* ItemUseMontage;
	
};
