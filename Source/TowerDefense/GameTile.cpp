// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTile.h"

// Sets default values
AGameTile::AGameTile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(USceneComponent::GetDefaultSceneRootVariableName());
    RootComponent = RootSceneComponent;
    
    Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
    Arrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGameTile::BeginPlay()
{
    Super::BeginPlay();
}
