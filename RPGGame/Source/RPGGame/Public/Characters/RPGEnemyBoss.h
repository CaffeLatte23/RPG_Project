// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Characters/RPGEnemyBase.h"
#include "RPGEnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API ARPGEnemyBoss : public ARPGEnemyBase
{
	GENERATED_BODY()

public:
    
	ARPGEnemyBoss();
    
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
};
