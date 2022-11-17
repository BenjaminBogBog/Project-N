// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNGameMode.h"
#include "EnemyAI.h"

void AProjectNGameMode::StartPlay()
{
	Super::StartPlay();

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ActorItr->ActorHasTag("WalkPoint"))
			WalkPoints.Push(*ActorItr);
	}

	for (int i = 0; i < initialEnemySpawnNum; i++) {
		EnemySpawn();
	}

	GetWorldTimerManager().SetTimer(EnemySpawnTimeHandle, this, &AProjectNGameMode::EnemySpawnBatch, EnemySpawnRate, true, EnemySpawnRate);
}

void AProjectNGameMode::EnemySpawnBatch() {
	for (int i = 0; i < EnemyBatchNum; i++) {
		EnemySpawn();
	}
}

void AProjectNGameMode::EnemySpawn()
{
	float Distance = FMath::FRandRange(MinSpawnArea, MaxSpawnArea);
	float XSpawn = FMath::FRandRange(-Distance, Distance);
	float YSpawn = FMath::FRandRange(-Distance, Distance);
	float ZSpawn = 0;

	if (PlayerActor) {
		XSpawn += PlayerActor->GetActorLocation().X;
		YSpawn += PlayerActor->GetActorLocation().Y;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PLAYER DONT EXIST"));
	}

	FHitResult hit;

	if (GetWorld()->LineTraceSingleByChannel(hit, FVector(XSpawn, YSpawn, 5000.0f), FVector(XSpawn, YSpawn, -5000.0f), ECC_Visibility)) {
		UE_LOG(LogTemp, Warning, TEXT("HIT %s at Vector3(%f, %f, %f)"), *hit.GetActor()->GetName(), hit.Location.X, hit.Location.Y, hit.Location.Z);
		ZSpawn = hit.Location.Z + 100.0f;
	}

	FVector SpawnLocation = FVector(XSpawn, YSpawn, ZSpawn);

	AActor* EnemyActor = GetWorld()->SpawnActor<AActor>(EnemyObjectToSpawn, SpawnLocation, FRotator3d(0, 0, 0));

	UE_LOG(LogTemp, Warning, TEXT("SPAWN ENEMY at Vector3(%f, %f, %f)"), hit.Location.X, hit.Location.Y, hit.Location.Z);
}
