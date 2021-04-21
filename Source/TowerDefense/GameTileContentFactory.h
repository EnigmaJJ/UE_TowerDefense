// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameTileContentFactory.generated.h"

enum class EGameTileContentType : uint8;

class AGameTileContent;

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UGameTileContentFactory : public UPrimaryDataAsset
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
};
