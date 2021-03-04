// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameBoardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UGameBoardComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UGameBoardComponent();

    void Initialize(FVector2D InSize);

private:
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
    AActor* Ground;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
    TSubclassOf<class AGameTile> GameTileClass;

    FVector2D Size;
    TArray<TWeakObjectPtr<AGameTile>> GameTiles;
};
