// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyFactory.h"
#include "GameTile.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::SpawnOn(AGameTile* GameTile)
{
	SetActorLocation(GameTile->GetActorLocation());
}

void AEnemy::SetOriginFactory(UEnemyFactory* InOriginFactory)
{
	checkfSlow(!OriginFactory->IsValid(), TEXT("Redefined origin factory!"));
	OriginFactory = MakeWeakObjectPtr(InOriginFactory);
}

