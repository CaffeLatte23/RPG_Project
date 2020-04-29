// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveFloorVolume.h"
#include "Characters/RPGPlayerCharacter.h"
#include "RPGGameMode.h"
#include "Components/BoxComponent.h"

// Sets default values
AMoveFloorVolume::AMoveFloorVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    MoveVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("MoveVolume"));

	MoveVolume->OnComponentBeginOverlap.AddDynamic(this , &AMoveFloorVolume::OnOverlapBegin);

}

void AMoveFloorVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ARPGPlayerCharacter* Player = Cast<ARPGPlayerCharacter>(OtherActor);

	if(Player)
    {
		GetWorldTimerManager().PauseTimer(Player->SearchHandle);
	}	
}