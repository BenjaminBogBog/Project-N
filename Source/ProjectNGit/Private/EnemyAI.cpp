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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
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

		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle) && !bWalkBoolDebounce) {
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyAI::AIMoveDelay, 2.0f, false);
			bWalkBoolDebounce = true;
		}
	}

	if (FVector::Dist(GetActorLocation(), WalkPointsActor[WalkPointIndex]->GetActorLocation()) <= AIStopDistance) {

		AIController->StopMovement();

		if (WalkPointIndex + 1 < WalkPointsActor.Num()) {

			if (bCanWalk) {
				AIController->MoveToActor(WalkPointsActor[WalkPointIndex + 1]);
				WalkPointIndex++;
				bCanWalk = false;
				bWalkBoolDebounce = false;
			}
			
		}
		else {

			if (bCanWalk) {
				WalkPointIndex = 0;
				AIController->MoveToActor(WalkPointsActor[WalkPointIndex]);
				bCanWalk = false;
				bWalkBoolDebounce = false;
			}
		}
		
	}



	intervalTime += DeltaTime;

	if (intervalTime >= pawnSense->SensingInterval) {

		if (LastSeen != nullptr && pawnSense != nullptr && currentAIState != EAIState::Attack) {

			if (!pawnSense->CouldSeePawn(LastSeen)) {
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("PATROL"));
				currentAIState = EAIState::Patrol;

				AIController->MoveToActor(WalkPointsActor[WalkPointIndex]);
				GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("PUSHING"));
				currentAIState = EAIState::Pushing;

				AIController->MoveToActor(LastSeen);
				GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

			}
		}

		intervalTime = 0;
	}

	if (currentAIState == EAIState::Pushing) {

		if (FVector::Dist(GetActorLocation(), LastSeen->GetActorLocation()) <= AIStopDistance) {
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("ATTACK"));

			AIController->StopMovement();

			currentAIState = EAIState::Attack;

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

void AEnemyAI::OnSeePawn(APawn* OtherPawn)
{

	if (OtherPawn->ActorHasTag("Player")) {
		LastSeen = OtherPawn;
	}
}

void AEnemyAI::AIMoveDelay() {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("WALK TRUE"));
	bCanWalk = true;
}



