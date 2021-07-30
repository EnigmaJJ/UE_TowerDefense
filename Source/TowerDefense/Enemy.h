// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:
	void SpawnOn(AGameTile* GameTile);

	void SetOriginFactory(UEnemyFactory* InOriginFactory);
	FORCEINLINE UEnemyFactory* GetOriginFactory() const { return OriginFactory.Get(); }

private:
	TWeakObjectPtr<UEnemyFactory> OriginFactory;
};
