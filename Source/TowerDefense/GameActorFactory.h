// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/Object.h"
#include "GameActorFactory.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOWERDEFENSE_API UGameActorFactory : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:
	template<typename T>
	T* SpawnGameActor(UClass* InClass) const;
};

template<typename T>
T* UGameActorFactory::SpawnGameActor(UClass* InClass) const
{
	UWorld* World = GetWorld();
	if (nullptr == World)
	{
		return nullptr;
	}

	return World->SpawnActor<T>(InClass);
}
