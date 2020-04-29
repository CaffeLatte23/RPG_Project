// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemPickUp.h"
#include "Items/RPGItem.h"
#include "Components/RPGInventoryComponent.h"
#include "Characters/RPGPlayerCharacter.h"

// Sets default values
AItemPickUp::AItemPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Root;
	Root->SetCapsuleRadius(20.f);
	Root->SetCapsuleHalfHeight(100.f);

	CollectCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollectCollision"));
	CollectCollision->SetupAttachment(Root);
	CollectCollision->SetSphereRadius(100.f);
	CollectCollision->OnComponentBeginOverlap.AddDynamic(this , &AItemPickUp::OnOverlapBegin);

	

}

// Called when the game starts or when spawned
void AItemPickUp::BeginPlay()
{
	Super::BeginPlay();

	if(Impulse.Size() > 0.f)
	{
        Root->AddImpulse(Impulse * 500.f , NAME_None , true);
	}
	
}

void AItemPickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{   
		PlayerCollecting = OtherActor;
        GiveItem();
	}
}

void AItemPickUp::GiveItem()
{
	ARPGPlayerCharacter* Player = Cast<ARPGPlayerCharacter>(PlayerCollecting);
    URPGInventoryComponent* InventoryComp = Player->InventoryComp;

	if(!InventoryComp)
	{   
		UE_LOG(LogRPG, Warning ,TEXT("InventoryComp null"));
		return;
	}
    
	UE_LOG(LogRPG, Warning ,TEXT("GiveItem : Called"));
	InventoryComp->AddInventoryItem(ItemType , Count);
    AnimationCollection();
    UGameplayStatics::PlaySoundAtLocation(this , PickUpSound , GetActorLocation());
}
