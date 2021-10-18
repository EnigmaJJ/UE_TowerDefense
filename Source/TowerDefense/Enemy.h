// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Direction.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class AGameTile;
class UEnemyFactory;

UCLASS()
class TOWERDEFENSE_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();
	
	virtual void Tick(float DeltaSeconds) override;

	void SpawnOn(AGameTile* GameTile);

	void SetOriginFactory(UEnemyFactory* InOriginFactory);
	FORCEINLINE UEnemyFactory* GetOriginFactory() const { return OriginFactory.Get(); }

	bool GameUpdate();

private:
	void PrepareIntro();
	void PrepareOutro();
	void PrepareNextState();
	void PrepareForward();
	void PrepareTurnRight();
	void PrepareTurnLeft();
	void PrepareTurnAround();

private:
	UPROPERTY(BlueprintReadWrite, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
	USceneComponent* ModelComponent;
	
	TWeakObjectPtr<UEnemyFactory> OriginFactory;

	TWeakObjectPtr<AGameTile> TileFrom, TileTo;
	FVector PositionFrom, PositionTo;
	FDirection::EType Dir;
	FDirection::EChangeType DirChange;
	float DirAngleFrom, DirAngleTo;
	float Progress, ProgressFactor;
};
