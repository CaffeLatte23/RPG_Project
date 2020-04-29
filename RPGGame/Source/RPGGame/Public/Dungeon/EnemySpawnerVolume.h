// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnerVolume.generated.h"

UCLASS()
class RPGGAME_API AEnemySpawnerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnerVolume();

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Spawner)
	class UBoxComponent* SpawnVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Spawner)
	int32 SpawnedNum = 0;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Spawner)
	TArray<TSubclassOf<class ARPGEnemyBase>> SpawnActors;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Spawner)
	TArray<class ARPGSpawnPoint*> SpawnLocations;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Spawner)
	TArray<class ASnapMapConnectionActor*> RoomDoors;

	UPROPERTY()
	bool bDoOneceCheck = false;

	UFUNCTION(BlueprintCallable ,Category = "Door")
	void AllDoorLock(bool bLock = true);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	void DefeatedActor();

};
