// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardComponent.h"
#include "GameTile.h"
#include "Math/UnitConversion.h"

// Sets default values for this component's properties
UGameBoardComponent::UGameBoardComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

void UGameBoardComponent::Initialize(FVector2D InSize)
{
    Size = FVector2D { FMath::Floor(InSize.X), FMath::Floor(InSize.Y) };
    Ground->SetActorScale3D(FVector(Size.X, Size.Y, 1.0f));

    GameTiles.Reserve(Size.X * Size.Y);
    
    UWorld* World = GetWorld();
    const FVector2D Offset { (Size.X - 1) * 0.5f, (Size.Y - 1) * 0.5f };
    for (int Y = 0; Y < static_cast<int>(Size.Y); ++Y)
    {
        for (int X = 0; X < static_cast<int>(Size.X); ++X)
        {
            FVector Location { FUnitConversion::Convert(X - Offset.X, EUnit::Meters, EUnit::Centimeters)
                             , FUnitConversion::Convert(Y - Offset.Y, EUnit::Meters, EUnit::Centimeters)
                             , 0.0f };
            AGameTile* GameTile = World->SpawnActor<AGameTile>(GameTileClass, Location, FRotator::ZeroRotator);
            GameTile->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);

            GameTiles.Add(MakeWeakObjectPtr(GameTile));
        }
    }
}
