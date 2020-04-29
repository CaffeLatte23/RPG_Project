// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyAIControllerBase.h"
#include "Characters/RPGEnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ARPGEnemyAIControllerBase::ARPGEnemyAIControllerBase()
{
    BBComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB Comp"));
}

void ARPGEnemyAIControllerBase::OnPossess(APawn* InPawn)
{   
    Super::OnPossess(InPawn);

    PosCharacter = Cast<ARPGEnemyBase>(InPawn);

    if(!BBData || !BTData || !PosCharacter)
    {   
        return; 
    }

    PosCharacter->OnSpawnFinished.BindUObject(this , &ARPGEnemyAIControllerBase::BehaiviorSetup);
    
}

void ARPGEnemyAIControllerBase::BehaiviorSetup()
{   

    this->UseBlackboard(BBData , BBComp);

    //攻撃時、プレイヤーにどこまで近づくか
    BBComp->SetValueAsFloat("AttackDistance" , ArrowDistance);
    
    //攻撃準備時、EQSの内側半径
    BBComp->SetValueAsFloat("InnerRadius" , InnerRadius);

    //攻撃準備時、EQSの外側半径
    BBComp->SetValueAsFloat("OuterRadius" , OuterRadius);

    //EQSリングの個数
    BBComp->SetValueAsInt("RingNum" , RingNum);

    //ランダム攻撃に行く確率の許容ライン
    BBComp->SetValueAsFloat("AttackArrowLine" , AttackArrowLine);
    
    //Behaivior 開始
    this->RunBehaviorTree(BTData);

}

