// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameActorFactory.h"
#include "GameTileContentFactory.generated.h"

enum class EGameTileContentType : uint8;

class AGameTileContent;

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UGameTileContentFactory : public UGameActorFactory
{
	GENERATED_BODY()

public:
	AGameTileContent* Get(EGameTileContentType Type);

	void Reclaim(AGameTileContent* GameTileContent);

private:
	AGameTileContent* Get(const TSubclassOf<AGameTileContent>& GameTileContentClass);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AGameTileContent> DestinationClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AGameTileContent> EmptyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AGameTileContent> WallClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AGameTileContent> SpawnPointClass;
};
