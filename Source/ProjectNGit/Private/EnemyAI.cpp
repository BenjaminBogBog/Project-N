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
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AAIController* AIController = Cast<AAIController>(GetController());
	//AIController->MoveToActor(Player->GetOwner());
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

