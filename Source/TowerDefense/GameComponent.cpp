// Fill out your copyright notice in the Description page of Project Settings.

#include "GameComponent.h"
#include "GameBoardComponent.h"
#include "GameTile.h"
#include "GameTileContent.h"
#include "GameTileContentFactory.h"

// Sets default values for this component's properties
UGameComponent::UGameComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGameComponent::BeginPlay()
{
	Super::BeginPlay();

	GameTileContentFactory = NewObject<UGameTileContentFactory>(this, GameTileContentFactoryClass);
	
	UGameBoardComponent* GameBoardComponent =
		CastChecked<UGameBoardComponent>(GameBoard->GetComponentByClass(UGameBoardComponent::StaticClass()));
	GameBoardComponent->Initialize(BoardSize, GameTileContentFactory);

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->SetShowMouseCursor(true);
}

void UGameComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController->WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		HandleTouch();
	}
}

#if WITH_EDITOR
void UGameComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (nullptr != PropertyChangedEvent.Property)
	{
		if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FVector2D, X))
		{
			if (BoardSize.X < 2.0f)
			{
				BoardSize.X = 2.0f;
			}
		}

		if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FVector2D, Y))
		{
			if (BoardSize.Y < 2.0f)
			{
				BoardSize.Y = 2.0f;
			}
		}
	}
}
#endif

void UGameComponent::HandleTouch()
{
	UGameBoardComponent* GameBoardComponent = GameBoard->FindComponentByClass<UGameBoardComponent>();
	AGameTile* GameTile = GameBoardComponent->GetTile();
	if (nullptr != GameTile)
	{
		GameBoardComponent->ToggleDestination(GameTile);
	}
}
