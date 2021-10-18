// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyCollection.generated.h"

class AEnemy;

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UEnemyCollection : public UObject
{
	GENERATED_BODY()

public:
	void Add(AEnemy* Enemy);

	void GameUpdate();

private:
	TArray<TWeakObjectPtr<AEnemy>> Enemies;
};
