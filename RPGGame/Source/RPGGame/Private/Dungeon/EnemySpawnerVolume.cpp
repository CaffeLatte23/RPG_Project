// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/EnemySpawnerVolume.h"
#include "Dungeon/RPGSpawnPoint.h"
#include "Dungeon/DungeonDoor.h"
#include "RPGGameMode.h"
#include "RPGPlayerControllerBase.h"
#include "Components/BoxComponent.h"
#include "Characters/RPGEnemyBase.h"
#include "DungeonArchitectRuntime/Public/Builders/SnapMap/Connection/SnapMapConnectionComponent.h"
#include "DungeonArchitectRuntime/Public/Builders/SnapMap/Connection/SnapMapConnectionActor.h"
#include "DungeonArchitectRuntime/Public/Builders/SnapMap/Connection/SnapMapConnectionInfo.h"

// Sets default values
AEnemySpawnerVolume::AEnemySpawnerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Spawn Volume"));
	SpawnVolume->OnComponentBeginOverlap.AddDynamic(this , &AEnemySpawnerVolume::OnOverlapBegin);
	SpawnVolume->OnComponentEndOverlap.AddDynamic(this , &AEnemySpawnerVolume::OnOverlapEnd);
	
}

// Called when the game starts or when spawned
void AEnemySpawnerVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawnerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnerVolume::AllDoorLock(bool bLock)
{
	for(ASnapMapConnectionActor* ConnectionActor : RoomDoors)
	{   
		USnapMapConnectionComponent* ConnectionComp = ConnectionActor->ConnectionComponent;

		//生成したのがドアか
		TArray<AActor*> OutArray;
            
		//ドアにアタッチされているアクターが自作クラスを継承しているか
		ConnectionActor->GetAttachedActors(OutArray);
		if(OutArray.Num() > 0)
		{
			for(AActor* Actor : OutArray)
			{
				ADungeonDoor* Door = Cast<ADungeonDoor>(Actor);
				if(Door)
				{  
					if(bLock)
					{
						Door->LockDoor();
					}
					else
					{
						Door->UnLockDoor();
					}
				}
				
			}
		}
	}
}

void AEnemySpawnerVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	if(OtherActor == UGameplayStatics::GetPlayerCharacter(this , 0) && !bDoOneceCheck)
	{
		SpawnEnemy();

	}
}

void AEnemySpawnerVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	

}

void AEnemySpawnerVolume::SpawnEnemy()
{   
	bool bMusicChange = false;

	//Spawn可能な場所がない場合、　可能な敵がない場合
    if(SpawnLocations.Num() <= 0 || SpawnActors.Num() <= 0 || SpawnedNum > 0)
	{
		return;
	}
    
	for(ARPGSpawnPoint* Point : SpawnLocations)
	{
        if(!Point)
		{
			return;
		}

	    TSubclassOf<ARPGEnemyBase> SelectedEnemyType = SpawnActors[FMath::RandRange(0,SpawnActors.Num() - 1 )];
	    FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Point->GetActorLocation() , UGameplayStatics::GetPlayerCharacter(this , 0)->GetActorLocation());

	    ARPGEnemyBase* Enemy = GetWorld()->SpawnActor<ARPGEnemyBase>(SelectedEnemyType , Point->GetActorLocation() , FRotator(0.f , SpawnRotation.Yaw , 0));
	    Enemy->SpawnDefaultController();
		Enemy->ParentVolume = this;
        
		SpawnedNum++;
		bMusicChange = true;
		bDoOneceCheck = true;
	}


	if(SpawnedNum > 0)
	{   
		AllDoorLock(true);
		
		ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(UGameplayStatics::GetPlayerController(this , 0 ));
		if(PlayerController)
		{   
			PlayerController->NotifyOnEnemyFight();
		}
	}
	
	//音楽の変更
	ARPGGameMode* GameMode = Cast<ARPGGameMode>(UGameplayStatics::GetGameMode(this));
	if(GameMode && bMusicChange)
	{
		GameMode->ChangeMusic(true);
	}
	
	
}

void AEnemySpawnerVolume::DefeatedActor()
{   
	SpawnedNum--;

	if(SpawnedNum == 0)
	{
	   AllDoorLock(false);
	   ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(UGameplayStatics::GetPlayerController(this , 0));
       FTimerHandle Handle;
	   GetWorldTimerManager().SetTimer(Handle , [=](){
		   if(PlayerController)
	       {
		        PlayerController->NotifyAllEnemiesDefeated();
	       }
	   } , 3.0f , false);
	   
	}
}

