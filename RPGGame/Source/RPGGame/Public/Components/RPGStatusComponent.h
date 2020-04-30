// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Components/ActorComponent.h"
#include "RPGStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGGAME_API URPGStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGStatusComponent();

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly ,Category = "Default")
    class UDataTable* DefaultTable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
    
	UPROPERTY()
    class ARPGCharacterBase* Owner;
	
	//オーナーのステータス変数
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
    FRPGStatus OwnerStatus;
    
	//スキルポイント割り振り用
	FRPGStatus SkillPointsStock;

	//スキルポイント
	int32 SkillPoint = 0;
    
	FOnStatusLoaded OnStatusLoaded;
    FOnStatusLoadedBP OnStatusLoadedBP;
	
	UFUNCTION()
	void AddDefaultStatus(int32 Lv);

	FRPGStatus LevelUp();

	UFUNCTION()
	void UpdateStatus();

	UFUNCTION()
	bool LoadStatus();

	UFUNCTION()
	bool SaveStatus();
    
	UFUNCTION(BlueprintPure , BlueprintCallable , Category = Status)
	FRPGStatus GetOwnerStatus();

		
};
