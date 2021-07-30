// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameActorFactory.h"
#include "EnemyFactory.generated.h"

class AEnemy;

/**
* 
*/
UCLASS()
class TOWERDEFENSE_API UEnemyFactory : public UGameActorFactory
{
	GENERATED_BODY()

public:
	AEnemy* Get();
	void Reclaim(AEnemy* Enemy);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AEnemy> EnemyClass;
};
