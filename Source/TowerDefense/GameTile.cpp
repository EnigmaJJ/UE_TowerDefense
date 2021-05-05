// Fill out your copyright notice in the Description page of Project Settings.

#include "GameTile.h"
#include "GameTileContent.h"

const FQuat AGameTile::NorthRotation = FQuat::MakeFromEuler(FVector { 0.0f, 0.0f, 90.0f });
const FQuat AGameTile::EastRotation = FQuat::MakeFromEuler(FVector { 0.0f, 0.0f, 180.0f });
const FQuat AGameTile::SouthRotation = FQuat::MakeFromEuler(FVector { 0.0f, 0.0f, 270.0f });
const FQuat AGameTile::WestRotation = FQuat::MakeFromEuler(FVector { 0.0f, 0.0f, 0.0f });

// Sets default values
AGameTile::AGameTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(USceneComponent::GetDefaultSceneRootVariableName());
	RootComponent = RootSceneComponent;
	
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
}

void AGameTile::MakeEastWestNeighbors(AGameTile* EastTile, AGameTile* WestTile)
{
	checkfSlow((!EastTile->WestTile->IsValid() && !WestTile->EastTile->IsValid()), TEXT("Redefined neighbors!"));
	
	EastTile->WestTile = MakeWeakObjectPtr(WestTile);
	WestTile->EastTile = MakeWeakObjectPtr(EastTile);
}

void AGameTile::MakeNorthSouthNeighbors(AGameTile* NorthTile, AGameTile* SouthTile)
{
	checkfSlow((!NorthTile->SouthTile->IsValid() && !SouthTile->NorthTile->IsValid()), TEXT("Redefined neighbors!"));

	NorthTile->SouthTile = MakeWeakObjectPtr(SouthTile);
	SouthTile->NorthTile = MakeWeakObjectPtr(NorthTile);
}

void AGameTile::SetContent(AGameTileContent* InGameTileContent)
{
	checkfSlow(nullptr != InGameTileContent, TEXT("Null assigned to content!"));
	
	if (GameTileContent.IsValid())
	{
		GameTileContent->Recycle();
		GameTileContent.Reset();
	}

	GameTileContent = MakeWeakObjectPtr(InGameTileContent);
	GameTileContent->SetActorLocation(GetActorLocation());
}

AGameTile* AGameTile::GrowPathNorth()
{
	return GrowPathTo(NorthTile.Get());
}

AGameTile* AGameTile::GrowPathEast()
{
	return GrowPathTo(EastTile.Get());
}

AGameTile* AGameTile::GrowPathSouth()
{
	return GrowPathTo(SouthTile.Get());
}

AGameTile* AGameTile::GrowPathWest()
{
	return GrowPathTo(WestTile.Get());
}

void AGameTile::BecomeDestination()
{
	NextTileOnPath.Reset();
	Distance = 0;
}

void AGameTile::ClearPath()
{
	NextTileOnPath.Reset();
	Distance = TNumericLimits<int32>::Max();
}

// Called when the game starts or when spawned
void AGameTile::BeginPlay()
{
	Super::BeginPlay();
}

AGameTile* AGameTile::GrowPathTo(AGameTile* NeighborTile)
{
	checkfSlow(NeighborTile->HasPath(), TEXT("No path!"));

	if ((nullptr == NeighborTile) || NeighborTile->HasPath())
	{
		return nullptr;
	}

	NeighborTile->Distance = Distance + 1;
	NeighborTile->NextTileOnPath = this;

	AGameTileContent* NeighborTileContent = NeighborTile->GetContent();
	if ((nullptr != NeighborTileContent) && (EGameTileContentType::Wall == NeighborTileContent->GetType()))
	{
		return nullptr;
	}
	
	return NeighborTile;
}

void AGameTile::ShowPath() const
{
	if (0 == Distance)  // 目的地
	{
		Arrow->SetVisibility(false);
		return;
	}

	Arrow->SetVisibility(true);
	Arrow->SetRelativeRotation(
	            NextTileOnPath == NorthTile ? NorthRotation :
	            NextTileOnPath == EastTile ? EastRotation :
	            NextTileOnPath == SouthTile ? SouthRotation :
	            WestRotation);
}

void AGameTile::HidePath() const
{
	Arrow->SetVisibility(false);
}
