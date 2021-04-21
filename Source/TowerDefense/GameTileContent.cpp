// Fill out your copyright notice in the Description page of Project Settings.

#include "GameTileContent.h"
#include "GameTileContentFactory.h"

// Sets default values
AGameTileContent::AGameTileContent()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

void AGameTileContent::Recycle()
{
    if (!OriginFactory.IsValid())
    {
        Destroy();
        return;
    }

    OriginFactory->Reclaim(this);
}

void AGameTileContent::SetOriginFactory(UGameTileContentFactory* InOriginFactory)
{
    checkfSlow(!OriginFactory->IsValid(), TEXT("Redefined origin factory!"));
    OriginFactory = MakeWeakObjectPtr(InOriginFactory);
}
