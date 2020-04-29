// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/DungeonDoor.h"

// Sets default values
ADungeonDoor::ADungeonDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    
}

// Called when the game starts or when spawned
void ADungeonDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADungeonDoor::LockDoor()
{   
    UE_LOG(LogRPG , Warning , TEXT("sdsads"));
	bDoorLock = true;
}

void ADungeonDoor::UnLockDoor()
{
	bDoorLock = false;
}


