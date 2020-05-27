// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RPGAIManageComponent.h"
#include "Dungeon/EnemySpawnerVolume.h"
#include "Characters/RPGEnemyBase.h"
#include "RPGGameMode.h"


// Sets default values for this component's properties
URPGAIManageComponent::URPGAIManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URPGAIManageComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningVolume = Cast<AEnemySpawnerVolume>(GetOwner());
	if(OwningVolume)
	{
		OwningVolume->RoomSpawnCompleted.BindUObject(this , &URPGAIManageComponent::StartManagement);
	}
}


// Called every frame
void URPGAIManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPGAIManageComponent::StartManagement(TArray<ARPGEnemyBase*> ActorsToManage)
{   
	UE_LOG(LogRPG , Warning , TEXT("Start AI Management"));
	
	
	//現在のゲームモードレベル設定の取得
	ARPGGameMode* GameMode = Cast<ARPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode)
	{
	   AttackArrowNum = GameMode->CombatNumRule.AttackArrowNum;
	   ReadyArrowNum = GameMode->CombatNumRule.ReadyArrowNum;
	   UE_LOG(LogRPG , Warning , TEXT("GameModeFind : %d") , AttackArrowNum);
	}
	else
	{
		UE_LOG(LogRPG , Warning , TEXT("Start AI Management"));
		
	}
	
    
	ManagedActors = ActorsToManage;
	RefreshManagement();

}

void URPGAIManageComponent::RefreshManagement()
{
	 //ソートの開始
    ManagedActors =  SortMapByDistance(ManagedActors);

}

void URPGAIManageComponent::NotifyDefeated(ARPGEnemyBase* Enemy)
{
	if(ManagedActors.Contains(Enemy))
	{
		ManagedActors.Remove(Enemy);
	}
	
}

ECombatType URPGAIManageComponent::GiveNewTask(ARPGEnemyBase* ReceivedActor)
{   
	//自分が現在、何番目に離れているかを取得
    RefreshManagement();
	int32 MyIdx = ManagedActors.Find(ReceivedActor);
    
    //自分の順番が攻撃サークルの設定数より後ろの場合
	if(MyIdx > ReadyArrowNum)
	{   
		//待機モードに移行
		
		return ECombatType::StandBy;
	}
	else //自分の順番が攻撃サークルに参加可能な順番の場合
	{
		//現在、攻撃を行っているキャラクターの数が設定数以下の場合、攻撃指令を与える可能性を付与
		if(GetSelectedActorsNum(ECombatType::Attack) < AttackArrowNum)
		{   
            if(UKismetMathLibrary::RandomBool())
			{   
				
				return ECombatType::Attack;
			}
		}
		else
		{
			UE_LOG(LogRPG , Warning , TEXT("Attack Num is over"));
		}
		

		return ECombatType::AttackReady;
	}
	
}

void URPGAIManageComponent::GetSelectedCombatActors(TArray<ARPGEnemyBase*>& SearchedActors,ECombatType TypeToSearch)
{   
	TArray<ARPGEnemyBase*> Actors = ManagedActors;
	for(ARPGEnemyBase* Actor : Actors)
	{
       if(Actor->CombatType == TypeToSearch)
	   {
		   SearchedActors.Add(Actor);
		   
	   }
	}
}

int32 URPGAIManageComponent::GetSelectedActorsNum(ECombatType TypeToSearch)
{
	TArray<ARPGEnemyBase*> SearchedActors;
	GetSelectedCombatActors(SearchedActors , TypeToSearch);

	return SearchedActors.Num();
}


TArray<ARPGEnemyBase*> URPGAIManageComponent::SortMapByDistance(TArray<ARPGEnemyBase*> ArrayToSort)
{   
	//編集用の配列への移動(後に削除するかも)
    TArray<ARPGEnemyBase*> SortedArray = ArrayToSort;
    //変更フラグ
	bool bDidSwap = false;
    
	//foreachだと要素数が純粋にはとれないため
	for(int32 idx = 0; idx < SortedArray.Num() - 1; idx++)
	{  
		for(int32 k = 0; k < SortedArray.Num() - idx - 1; k++)
		{   
			//比較する距離
			float FDistance = SortedArray[k]->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(this , 0));
			float BDistance = SortedArray[k + 1]->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(this , 0));

			if(FDistance > BDistance)
			{
				SortedArray.Swap(k , k+1);
                bDidSwap = true;
			}
		}
        
		//変更がなければソート終了
		if(!bDidSwap)
		{
			break;
		}
	}
	
    return SortedArray;

}

