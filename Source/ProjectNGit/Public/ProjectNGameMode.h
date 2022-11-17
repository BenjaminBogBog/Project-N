// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectNGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNGIT_API AProjectNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	FTimerHandle EnemySpawnTimeHandle;

	void EnemySpawn();

protected:
	virtual void StartPlay() override;
	
};
