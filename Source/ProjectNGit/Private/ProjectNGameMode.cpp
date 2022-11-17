// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNGameMode.h"

void AProjectNGameMode::StartPlay()
{
	Super::StartPlay();

	UE_LOG(LogTemp, Warning, TEXT("STARTING TO PLAY"));

	GetWorldTimerManager().SetTimer(EnemySpawnTimeHandle, this, &AProjectNGameMode::EnemySpawn, 10.0f, true, 0.0f);
}

void AProjectNGameMode::EnemySpawn()
{
	UE_LOG(LogTemp, Warning, TEXT("SPAWN ENEMY"));
}
