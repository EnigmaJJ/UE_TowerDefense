// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCollection.h"
#include "Enemy.h"

void UEnemyCollection::Add(AEnemy* Enemy)
{
	Enemies.Add(Enemy);
}

void UEnemyCollection::GameUpdate()
{
	for (int i = Enemies.Num() - 1; i >= 0; --i)
	{
		const TWeakObjectPtr<AEnemy>& Enemy = Enemies[i];
		if (!Enemy.IsValid() || !Enemy->GameUpdate())
		{
			Enemies.RemoveAt(i);
		}
	}
}
