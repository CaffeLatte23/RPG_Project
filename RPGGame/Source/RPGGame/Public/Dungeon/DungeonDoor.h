// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "GameFramework/Actor.h"
#include "DungeonDoor.generated.h"

UCLASS()
class RPGGAME_API ADungeonDoor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADungeonDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadWrite , Category = "Door")
	bool bDoorLock = false;
    
	UFUNCTION(BlueprintCallable , Category = "Door")
	void LockDoor();

	UFUNCTION(BlueprintCallable , Category = "Door")
	void UnLockDoor();

};
