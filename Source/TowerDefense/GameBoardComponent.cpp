// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardComponent.h"
#include "GameTile.h"
#include "GameTileContent.h"
#include "GameTileContentFactory.h"
#include "Math/UnitConversion.h"

// Sets default values for this component's properties
UGameBoardComponent::UGameBoardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGameBoardComponent::Initialize(FVector2D InSize, UGameTileContentFactory* InGameTileContentFactory)
{
	Size = FVector2D { FMath::Floor(InSize.X), FMath::Floor(InSize.Y) };
	Ground->SetActorScale3D(FVector(Size.X, Size.Y, 1.0f));

	GameTileContentFactory = MakeWeakObjectPtr(InGameTileContentFactory);

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

			GameTile->SetContent(GameTileContentFactory->Get(EGameTileContentType::Empty));
		}
	}

	ToggleDestination(GameTiles[GameTiles.Num() / 2].Get());
}

AGameTile* UGameBoardComponent::GetTile() const
{
	UWorld* World = GetWorld();
	checkSlow(nullptr != World);
	
	FHitResult HitResult;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, HitResult))
	{
		const FVector Location { FUnitConversion::Convert(HitResult.Location.X, EUnit::Centimeters, EUnit::Meters)
		                       , FUnitConversion::Convert(HitResult.Location.Y, EUnit::Centimeters, EUnit::Meters)
		                       , 0.0f };
		const int32 X = static_cast<int32>(Location.X + (Size.X * 0.5f));
		const int32 Y = static_cast<int32>(Location.Y + (Size.Y * 0.5f));
		if ((X >= 0) && (X < Size.X) && (Y >= 0) && (Y < Size.Y))
		{
			return GameTiles[X + (Y * Size.X)].Get();
		}
	}
	
	return nullptr;
}

void UGameBoardComponent::ToggleDestination(AGameTile* GameTile)
{
	if (nullptr == GameTile)
	{
		return;
	}
	
	if (EGameTileContentType::Destination == GameTile->GetContent()->GetType())
	{
		GameTile->SetContent(GameTileContentFactory->Get(EGameTileContentType::Empty));
		if (!FindPaths())
		{
			GameTile->SetContent(GameTileContentFactory->Get(EGameTileContentType::Destination));
			FindPaths();
		}
	}
	else
	{
		GameTile->SetContent(GameTileContentFactory->Get(EGameTileContentType::Destination));
		FindPaths();
	}
}

bool UGameBoardComponent::FindPaths()
{
	for (auto Iter = GameTiles.CreateIterator(); Iter; ++Iter)
	{
		TWeakObjectPtr<AGameTile> GameTile = *Iter;
		checkSlow(GameTile.IsValid());

		if (EGameTileContentType::Destination == GameTile->GetContent()->GetType())
		{
			GameTile->BecomeDestination();
			SearchFrontier.Enqueue(GameTile);
		}
		else
		{
			GameTile->ClearPath();
		}
	}

	if (SearchFrontier.IsEmpty())
	{
		return false;
	}

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

	return true;
}
