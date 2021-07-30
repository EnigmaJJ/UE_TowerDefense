// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyFactory.h"
#include "Enemy.h"

AEnemy* UEnemyFactory::Get()
{
	AEnemy* Enemy = SpawnGameActor<AEnemy>(EnemyClass);
	Enemy->SetOriginFactory(this);

	return Enemy;
}

void UEnemyFactory::Reclaim(AEnemy* Enemy)
{
	checkfSlow(this == Enemy->GetOriginFactory(), TEXT("Wrong factory reclaimed!"));
	Enemy->Destroy();
}
