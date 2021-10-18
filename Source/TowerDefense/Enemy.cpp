// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Direction.h"
#include "EnemyFactory.h"
#include "GameTile.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GameUpdate();
}

void AEnemy::SpawnOn(AGameTile* GameTile)
{
	checkfSlow(nullptr != GameTile->GetNextTileOnPath(), TEXT("Nowhere to go!"));
	
	TileFrom = GameTile;
	TileTo = GameTile->GetNextTileOnPath();
	Progress = 0.0f;

	PrepareIntro();
}

void AEnemy::SetOriginFactory(UEnemyFactory* InOriginFactory)
{
	checkfSlow(!OriginFactory->IsValid(), TEXT("Redefined origin factory!"));
	OriginFactory = MakeWeakObjectPtr(InOriginFactory);
}

bool AEnemy::GameUpdate()
{
	Progress += (GetWorld()->DeltaTimeSeconds * ProgressFactor);
	while (Progress >= 1.0f)
	{
		if (nullptr == TileTo)
		{
			OriginFactory->Reclaim(this);
			return false;
		}

		Progress = (Progress - 1.0f) / ProgressFactor;
		PrepareNextState();
		Progress *= ProgressFactor;
	}

	if (FDirection::EChangeType::None == DirChange)
	{
		SetActorLocation(FMath::Lerp(PositionFrom, PositionTo, Progress));
	}
	else
	{
		const float angle = FMath::Lerp(DirAngleFrom, DirAngleTo, Progress);
		SetActorRotation(FRotator::MakeFromEuler(FVector{ 0.0f, 0.0f, angle }));
	}
	
	return true;
}

void AEnemy::PrepareIntro()
{
	PositionFrom = TileFrom->GetActorLocation();
	PositionTo = TileFrom->GetExitPoint();
	Dir = TileFrom->GetPathDirection();
	DirChange = FDirection::EChangeType::None;
	DirAngleFrom = DirAngleTo = FDirection::GetAngle(Dir);
	SetActorRotation(FDirection::GetRotation(Dir));
	ProgressFactor = 2.0f;
}

void AEnemy::PrepareOutro()
{
	PositionTo = TileFrom->GetActorLocation();
	DirChange = FDirection::EChangeType::None;
	DirAngleTo = FDirection::GetAngle(Dir);
	ModelComponent->SetRelativeLocation(FVector::ZeroVector);
	SetActorRotation(FDirection::GetRotation(Dir));
	ProgressFactor = 2.0f;
}

void AEnemy::PrepareNextState()
{
	TileFrom = TileTo;
	TileTo = TileTo->GetNextTileOnPath();
	PositionFrom = PositionTo;
	if (nullptr == TileTo)
	{
		PrepareOutro();
		return;
	}
	PositionTo = TileFrom->GetExitPoint();
	DirChange = FDirection::GetDirectionChangeTo(Dir, TileFrom->GetPathDirection());
	Dir = TileFrom->GetPathDirection();
	DirAngleFrom = DirAngleTo;

	switch (DirChange)
	{
	case FDirection::EChangeType::None:
		PrepareForward();
		break;

	case FDirection::EChangeType::TurnRight:
		PrepareTurnRight();
		break;

	case FDirection::EChangeType::TurnLeft:
		PrepareTurnLeft();
		break;

	default:
		PrepareTurnAround();
		break;
	}
}

void AEnemy::PrepareForward()
{
	SetActorRotation(FDirection::GetRotation(Dir));
	DirAngleTo = FDirection::GetAngle(Dir);
	ModelComponent->SetRelativeLocation(FVector::ZeroVector);
	ProgressFactor = 1.0f;
}

void AEnemy::PrepareTurnRight()
{
	DirAngleTo = DirAngleFrom + 90.0f;
	ModelComponent->SetRelativeLocation(FVector{ 0.0f, -0.5f * GetWorldSettings()->WorldToMeters, 0.0f });
	SetActorLocation(PositionFrom + (FDirection::GetHalfVector(Dir) * GetWorldSettings()->WorldToMeters));
	ProgressFactor = 1.0f / (0.25f * PI);
}

void AEnemy::PrepareTurnLeft()
{
	DirAngleTo = DirAngleFrom - 90.0f;
	ModelComponent->SetRelativeLocation(FVector{ 0.0f, 0.5f * GetWorldSettings()->WorldToMeters, 0.0f });
	SetActorLocation(PositionFrom + (FDirection::GetHalfVector(Dir) * GetWorldSettings()->WorldToMeters));
	ProgressFactor = 1.0f / (0.25f * PI);
}

void AEnemy::PrepareTurnAround()
{
	DirAngleTo = DirAngleFrom + 180.0f;
	ModelComponent->SetRelativeLocation(FVector::ZeroVector);
	SetActorLocation(PositionFrom);
	ProgressFactor = 2.0f;
}
