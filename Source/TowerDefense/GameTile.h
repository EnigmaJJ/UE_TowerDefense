// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTile.generated.h"

class AGameTileContent;

UCLASS()
class TOWERDEFENSE_API AGameTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameTile();

	static void MakeEastWestNeighbors(AGameTile* EastTile, AGameTile* WestTile);
	static void MakeNorthSouthNeighbors(AGameTile* NorthTile, AGameTile* SouthTile);

	FORCEINLINE bool HasPath() const { return Distance != TNumericLimits<int32>::Max(); }

	FORCEINLINE bool IsAlternative() const { return bIsAlternative; }
	FORCEINLINE void SetIsAlternative(bool bInIsAlternative) { bIsAlternative = bInIsAlternative; }

	FORCEINLINE AGameTileContent* GetContent() const { return GameTileContent.Get(); }
	void SetContent(AGameTileContent* InGameTileContent);

	AGameTile* GrowPathNorth();
	AGameTile* GrowPathEast();
	AGameTile* GrowPathSouth();
	AGameTile* GrowPathWest();
	
	void BecomeDestination();
	void ClearPath();
	void ShowPath() const;
	void HidePath() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AGameTile* GrowPathTo(AGameTile* NeighborTile);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Arrow;

	static const FQuat NorthRotation;
	static const FQuat EastRotation;
	static const FQuat SouthRotation;
	static const FQuat WestRotation;

	TWeakObjectPtr<AGameTile> NorthTile, EastTile, SouthTile, WestTile;
	TWeakObjectPtr<AGameTile> NextTileOnPath;
	int32 Distance;

	bool bIsAlternative;
	TWeakObjectPtr<AGameTileContent> GameTileContent;
};
