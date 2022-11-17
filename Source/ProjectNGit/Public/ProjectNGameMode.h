// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EngineUtils.h"
#include "ProjectNGameMode.generated.h"

class AEnemyAI;

/**
 * 
 */
UCLASS()
class PROJECTNGIT_API AProjectNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	FTimerHandle EnemySpawnTimeHandle;
	AActor* PlayerActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
	TArray<AActor*> WalkPoints;

	//Function which holds code for individual enemy spawning
	void EnemySpawn();

	//Function which calls EnemySpawn in batches given a number from (int)EnemyBatchNum
	void EnemySpawnBatch();

	//How many enemies to spawn at the start, the initial batch of enemies to spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		int initialEnemySpawnNum;

	//Batch of enemies to spawn each interval
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		int EnemyBatchNum;

	//How much time before spawning the next batch of enemies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		float EnemySpawnRate;

	//The minimum distance of Enemy Spawn Point
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		float MinSpawnArea;

	//The maximum distance of Enemy Spawn Point
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		float MaxSpawnArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSettings")
		TSubclassOf<AActor> EnemyObjectToSpawn;

protected:
	virtual void StartPlay() override;
	
};
