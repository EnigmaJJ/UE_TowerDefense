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
    for (int Idx = 0, Y = 0; Y < static_cast<int>(Size.Y); ++Y)
    {
        for (int X = 0; X < static_cast<int>(Size.X); ++X, ++Idx)
        {
            FVector Location { FUnitConversion::Convert(X - Offset.X, EUnit::Meters, EUnit::Centimeters)
                             , FUnitConversion::Convert(Y - Offset.Y, EUnit::Meters, EUnit::Centimeters)
                             , 0.0f };
            AGameTile* GameTile = World->SpawnActor<AGameTile>(GameTileClass, Location, FRotator::ZeroRotator);
            GameTile->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);

            GameTiles.Add(MakeWeakObjectPtr(GameTile));

            // 平铺格所在的行大于零，则能够在当前平铺格和上一个平铺格之间建立南北相邻关系
            if (X > 0)
            {
                AGameTile::MakeNorthSouthNeighbors(GameTile, GameTiles[Idx - 1].Get());
            }
            
            // 平铺格所在的列大于零，则能够在当前平铺格和上一列同行的平铺格之间建立东西相邻关系
            if (Y > 0)
            {
                AGameTile::MakeEastWestNeighbors(GameTile, GameTiles[Idx - static_cast<int>(Size.X)].Get());
            }

            GameTile->SetIsAlternative(0 == (X & 1));
            if (0 == (Y & 1))
            {
                GameTile->SetIsAlternative(!GameTile->IsAlternative());
            }
        }
    }

    FindPaths();
}

void UGameBoardComponent::FindPaths()
{
    for (auto Iter = GameTiles.CreateIterator(); Iter; ++Iter)
    {
        TWeakObjectPtr<AGameTile> GameTile = *Iter;
        checkSlow(GameTile.IsValid());
        GameTile->ClearPath();
    }

    GameTiles[GameTiles.Num() / 2]->BecomeDestination();
    SearchFrontier.Enqueue(GameTiles[GameTiles.Num() / 2]);

    while (!SearchFrontier.IsEmpty())
    {
        TWeakObjectPtr<AGameTile> GameTile;
        SearchFrontier.Dequeue(GameTile);
        if (GameTile.IsValid())
        {
            if (GameTile->IsAlternative())
            {
                SearchFrontier.Enqueue(GameTile->GrowPathNorth());
                SearchFrontier.Enqueue(GameTile->GrowPathSouth());
                SearchFrontier.Enqueue(GameTile->GrowPathEast());
                SearchFrontier.Enqueue(GameTile->GrowPathWest());
            }
            else
            {
                SearchFrontier.Enqueue(GameTile->GrowPathWest());
                SearchFrontier.Enqueue(GameTile->GrowPathEast());
                SearchFrontier.Enqueue(GameTile->GrowPathSouth());
                SearchFrontier.Enqueue(GameTile->GrowPathNorth());
            }
        }
    }

    for (auto Iter = GameTiles.CreateIterator(); Iter; ++Iter)
    {
        const TWeakObjectPtr<AGameTile> GameTile = *Iter;
        GameTile->ShowPath();
    }
}
