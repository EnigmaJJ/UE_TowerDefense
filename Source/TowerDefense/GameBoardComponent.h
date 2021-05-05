// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameBoardComponent.generated.h"

class AGameTile;
class UGameTileContentFactory;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UGameBoardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameBoardComponent();

	void Initialize(FVector2D InSize, UGameTileContentFactory* InGameTileContentFactory);
	AGameTile* GetTile() const;
	
	void ToggleDestination(AGameTile* GameTile);
	void ToggleWall(AGameTile* GameTile);

	FORCEINLINE bool IsShowGrid() const { return bIsShowGrid; }
	void SetShowGrid(bool bShowGrid);

	FORCEINLINE bool IsShowPaths() const { return bIsShowPaths; }
	void SetShowPaths(bool bShowPaths);

private:
	bool FindPaths();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
	AActor* Ground;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AGameTile> GameTileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
	UTexture2D* GridTexture;

	FVector2D Size;
	TArray<TWeakObjectPtr<AGameTile>> GameTiles;
	TQueue<TWeakObjectPtr<AGameTile>> SearchFrontier;

	TWeakObjectPtr<UGameTileContentFactory> GameTileContentFactory;

private:
	static FName GridTextureParamName;
	static FName GridTexCoordScaleName;
	static UTexture* EngineWhiteSquareTexture;

	UPROPERTY()
	UMaterialInstanceDynamic* GroundMaterialInstance;
	
	bool bIsShowGrid;
	bool bIsShowPaths;
};
