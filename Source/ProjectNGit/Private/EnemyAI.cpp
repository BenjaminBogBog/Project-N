// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AAIController* AIController = Cast<AAIController>(GetController());
	WalkPointIndex = 0;
	AIController->MoveToActor(WalkPointsActor[WalkPointIndex]);
	bCanWalk = false;
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AAIController* AIController = Cast<AAIController>(GetController());

	FTimerHandle TimerHandle;

	if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle) {

		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle)) {
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyAI::AIMoveDelay, 2.0f, false);
		}
	}

	if (FVector::Dist(GetActorLocation(), WalkPointsActor[WalkPointIndex]->GetActorLocation()) <= AIStopDistance) {

		AIController->StopMovement();

		if (WalkPointIndex + 1 < WalkPointsActor.Num()) {

			if (bCanWalk) {
				AIController->MoveToActor(WalkPointsActor[WalkPointIndex + 1]);
				WalkPointIndex++;
				bCanWalk = false;
			}
			
		}
		else {

			if (bCanWalk) {
				AIController->MoveToActor(WalkPointsActor[0]);
				WalkPointIndex = 1;
				bCanWalk = false;
			}
		}
		
	}

	
}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAI::ApplyDamage(float damageToApply)
{
	if (BloodSplatterFX != nullptr)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatterFX, GetActorLocation());

	CurrentHealth -= damageToApply;

	if (CurrentHealth <= 0) {
		Destroy();
	}
}

void AEnemyAI::AIMoveDelay() {
	UE_LOG(LogTemp, Warning, TEXT("Set Walk to true"));
	bCanWalk = true;
}

