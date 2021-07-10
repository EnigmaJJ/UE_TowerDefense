// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTileContent.generated.h"

class UGameTileContentFactory;

UENUM(BlueprintType)
enum class EGameTileContentType : uint8
{
	Empty,
	Destination,
	Wall,
	SpawnPoint
};

UCLASS()
class TOWERDEFENSE_API AGameTileContent : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameTileContent();

	void Recycle();

	FORCEINLINE EGameTileContentType GetType() const { return Type; }

	void SetOriginFactory(UGameTileContentFactory* InOriginFactory);
	FORCEINLINE UGameTileContentFactory* GetOriginFactory() const { return OriginFactory.Get(); }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	EGameTileContentType Type;

	TWeakObjectPtr<UGameTileContentFactory> OriginFactory;
};
