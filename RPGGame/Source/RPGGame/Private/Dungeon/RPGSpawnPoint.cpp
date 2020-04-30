// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/RPGSpawnPoint.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ARPGSpawnPoint::ARPGSpawnPoint()
{
	BillBoard = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard"));
	/*static ConstructorHelpers::FObjectFinder<UTexture2D> TextureFile(TEXT("Texture2D'/Game/RPGGame/Assets/Point_Spawn.Point_Spawn'"));
	if(TextureFile.Object)
	{
		BillBoard->Sprite = TextureFile.Object;
	}*/
	
}

// Called when the game starts or when spawned
void ARPGSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}


