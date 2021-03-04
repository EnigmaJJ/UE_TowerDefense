// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UGameComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UGameComponent();

    virtual void BeginPlay() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
    FVector2D BoardSize = FVector2D {11.0f, 11.0f};

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=TowerDefense, meta=(AllowPrivateAccess="true"))
    AActor* GameBoard;
};
