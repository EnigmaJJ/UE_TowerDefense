// Fill out your copyright notice in the Description page of Project Settings.

#include "GameTileContentFactory.h"
#include "GameTileContent.h"

AGameTileContent* UGameTileContentFactory::Get(EGameTileContentType Type)
{
	switch (Type)
	{
	case EGameTileContentType::Destination:
		return Get(DestinationClass);

	case EGameTileContentType::Empty:
		return Get(EmptyClass);

	case EGameTileContentType::Wall:
		return Get(WallClass);

	case EGameTileContentType::SpawnPoint:
		return Get(SpawnPointClass);

	default:
		checkfSlow(false, TEXT("Unsupported type: %d"), static_cast<int32>(Type));
		return nullptr;
	}
}

void UGameTileContentFactory::Reclaim(AGameTileContent* GameTileContent)
{
	checkfSlow(this == GameTileContent->GetOriginFactory(), TEXT("Wrong factory reclaimed!"));
	GameTileContent->Destroy();
}

AGameTileContent* UGameTileContentFactory::Get(const TSubclassOf<AGameTileContent>& GameTileContentClass)
{
	AGameTileContent* GameTileContent = SpawnGameActor<AGameTileContent>(GameTileContentClass);
	GameTileContent->SetOriginFactory(this);

	return GameTileContent;
}
