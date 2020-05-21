// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public: 
    ARPGGameMode();
    virtual void BeginPlay() override;
    
    FAsyncSaveGameToSlotDelegate OnCompleted;

    UPROPERTY()
    class UDataTable* CombatRuleTable;

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = GameMode)
    int32 FloorNum = 1;

    UPROPERTY(EditAnyWhere , Category = Default)
    FName LevelName;

    FAICombatRule CombatNumRule;

    UPROPERTY(BlueprintReadWrite , Category = SaveGame)
    class URPGSaveGame* SaveGame;

    UPROPERTY(BlueprintReadWrite , Category = Default)
    class UUserWidget* StartWidget;

    UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Default)
    TSubclassOf<class UUSerWidget> FloorStartWidget;

    UFUNCTION()
    void GetGameModeRule(EGameMode GameLevel);

    UFUNCTION(BlueprintImplementableEvent ,BlueprintCallable ,  Category = Sound)
    void ChangeMusic(bool bIsConnect);

    UFUNCTION(BlueprintCallable , Category = GameMode)
    void MoveNextFloor();

    UFUNCTION(BlueprintCallable , Category = GameMode)
    void JumpToFloor(FName Name);

    UFUNCTION()
    void MoveToLevel(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	
};
