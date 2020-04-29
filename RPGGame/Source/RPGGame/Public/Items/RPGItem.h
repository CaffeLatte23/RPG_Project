// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "RPGAssetManager.h"
#include "RPGItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API URPGItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

    URPGItem()
	    : Price(0)
		, MaxCount(1)
		, MaxLevel(1)
	{}

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
    FText ItemName;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
    FText ItemDescription;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
    FSlateBrush ItemIcon;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
    int32 Price;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
    int32 MaxCount;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
    int32 MaxLevel;

	UFUNCTION()
	bool IsConsumable() const;

	UFUNCTION()
	FString GetIdentifierString() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
