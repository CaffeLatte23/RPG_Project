// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Components/ActorComponent.h"
#include "RPGAIManageComponent.generated.h"

class ARPGEnemyBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGGAME_API URPGAIManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGAIManageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly , Category = AI)
	class AEnemySpawnerVolume* OwningVolume;

	UPROPERTY(BlueprintReadOnly , Category = AI)
	TArray< ARPGEnemyBase* > ManagedActors;

	UPROPERTY(BlueprintReadWrite , Category = AI)
	int32 AttackArrowNum = 0;

	UPROPERTY(BlueprintReadWrite , Category = AI)
	int32 ReadyArrowNum = 0;
    
	//マネージメントの開始関数
	UFUNCTION(BlueprintCallable , Category = AI)
    void StartManagement(TArray<ARPGEnemyBase*> ActorsToManage);

	UFUNCTION(BlueprintCallable , Category = AI)
	void RefreshManagement();

	UFUNCTION(BlueprintCallable , Category = AI)
	ECombatType GiveNewTask(ARPGEnemyBase* ReceivedActor);
    
	//選んだコンバットタイプのアクターを取得する関数
	UFUNCTION(BlueprintCallable , Category = AI)
	void GetSelectedCombatActors(TArray<ARPGEnemyBase*>& SearchedActors , ECombatType TypeToSearch);

	UFUNCTION(BlueprintCallable , Category = AI)
	int32 GetSelectedActorsNum(ECombatType TypeToSearch);
    
	//距離での配列ソート
	UFUNCTION(BlueprintCallable , Category = AI)
	TArray<ARPGEnemyBase*> SortMapByDistance(TArray<ARPGEnemyBase*> ArrayToSort);

	UFUNCTION()
	void NotifyDefeated(class ARPGEnemyBase* Enemy);
};
