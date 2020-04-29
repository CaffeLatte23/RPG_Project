// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UI_HUD.h"
#include "RPGPlayerControllerBase.h"
#include "Components/VerticalBox.h"

void UUI_HUD::NativeConstruct()
{
    Statsgauge = Cast<UVerticalBox>(GetWidgetFromName("ゲージBox"));
    
    ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(UGameplayStatics::GetPlayerController(this , 0));
    if(PlayerController)
    {
        PlayerController->OnAllEnemiesDefeated.AddUObject(this , &UUI_HUD::AllDefeated);
        PlayerController->OnEnemyFight.AddUObject(this , &UUI_HUD::EnemyOnFight);
    }
}

bool UUI_HUD::IsGaugeVisible()
{
   if(Statsgauge == nullptr)
   {
       return true;
   }

   return (Statsgauge->GetVisibility() == ESlateVisibility::Visible);
}

