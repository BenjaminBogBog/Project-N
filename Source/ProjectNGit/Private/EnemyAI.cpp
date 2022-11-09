// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	pawnSense->RegisterComponent();

	pawnSense->OnSeePawn.AddDynamic(this, &AEnemyAI::OnSeePawn);
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	if (pawnSense != nullptr) {
		pawnSense->bHearNoises = false;
		UE_LOG(LogTemp, Warning, TEXT("PAWN SENSE: %f"), pawnSense->HearingThreshold);
	}
	
	CurrentHealth = MaxHealth;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	AIController = Cast<AAIController>(GetController());

	currentAIState = EAIState::Patrol;
	WalkPointIndex = 0;
	AIController->MoveToActor(WalkPointsActor[WalkPointIndex]);
	bCanWalk = false;
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FTimerHandle TimerHandle;

	LastSeen = GetWorld()->GetFirstPlayerController()->GetPawn();

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
				WalkPointIndex = 0;
				AIController->MoveToActor(WalkPointsActor[0]);
				bCanWalk = false;
			}
		}
		
	}



	intervalTime += DeltaTime;

	if (intervalTime >= pawnSense->SensingInterval) {

		if (LastSeen != nullptr && pawnSense != nullptr) {

			if (!pawnSense->CouldSeePawn(LastSeen)) {
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("PATROL"));
				currentAIState = EAIState::Patrol;

				AIController->MoveToActor(WalkPointsActor[WalkPointIndex]);
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("ATTACK"));
				currentAIState = EAIState::Attack;

				AIController->MoveToActor(Player);
			}
		}

		intervalTime = 0;
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

void AEnemyAI::OnSeePawn(APawn* OtherPawn)
{

	if (OtherPawn->ActorHasTag("Player")) {
		LastSeen = OtherPawn;
	}
}

void AEnemyAI::AIMoveDelay() {
	bCanWalk = true;
}



