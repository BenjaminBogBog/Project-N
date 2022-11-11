// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creating a Pawn Sensing Component to enable sensing
	pawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	pawnSense->RegisterComponent();

		//Binds Function to the OnSeePawn delegate
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

	//Finding HitBoxComponent and Binding a Function to the OnBeginOverlap
	USceneComponent* hitboxComp = GetMesh()->GetChildComponent(0);
	UBoxComponent* hitbox = Cast<UBoxComponent>(hitboxComp);

	if (hitbox != nullptr)
		hitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::OnBeginOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find box Component"));

	//Setting INIT values
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

	//Assigns the Player variable to the Only Player Character in the Game, must change if want to implement Online
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//A TimerHandle which holds the timer content
	FTimerHandle TimerHandle;

	//When AI is Idle, meaning it has reached a WalkPoint, start a timer for 2.0 seconds before continuing its walk path
	if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle) {

		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle) && !bWalkBoolDebounce) {
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyAI::AIMoveDelay, 2.0f, false);
			bWalkBoolDebounce = true;
		}
	}

	//Stop AI when the distance is less than **AIStopDistance** meters from the current walkpoint
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

	//Adding DeltaTime every tick to make an accurate timer
	intervalTime += DeltaTime;



	//AI MOVEMENT
	
	//Execute when every sensing interval instead of every tick
	//This IF holds the content for whether the AI sees the player or not. If it does, it rushes towards the player. If not it goes between walkpoints
	if (intervalTime >= pawnSense->SensingInterval) {

		if (LastSeen != nullptr && pawnSense != nullptr && currentAIState != EAIState::Attack && !bRecentlyHit) {

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

		//Resets time variable once finishes executing
		intervalTime = 0;
	}

	//AI ATTACK
	
	//Executes when AI sees players, starts rushing towards player
	//Once close enough AI will start Attacking
	if (currentAIState == EAIState::Pushing && !bRecentlyHit) {

		if (FVector::Dist(GetActorLocation(), LastSeen->GetActorLocation()) <= AIStopDistance) {
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("ATTACK"));

			AIController->StopMovement();

			currentAIState = EAIState::Attack;

			bIsAttacking = true;

			//Perform Animation
			PlayAnimMontage(AttackAnimationMontage);
		}
	}

	//Check if the AI has finished attack animation
	if (bIsAttacking && GetMesh()->GetAnimInstance()->Montage_GetIsStopped(AttackAnimationMontage)) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("DONE ATTACK"));

		currentAIState = EAIState::Patrol;
		bIsAttacking = false;
	}

	//Check if the AI has finished hit animation
	if (bRecentlyHit && GetMesh()->GetAnimInstance()->Montage_GetIsStopped(HitAnimationMontage)) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("DONE HIT"));

		currentAIState = EAIState::Patrol;
		bRecentlyHit = false;
	}
	
}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Delegate called when AISense sees a pawn
void AEnemyAI::OnSeePawn(APawn* OtherPawn)
{
	//Assign LastSeen to the referred pawn whenever AI sees player, 
	//this is used in other parts of the code to check if AI should attack to the player or not
	if (OtherPawn->ActorHasTag("Player")) {
		LastSeen = OtherPawn;
	}
}

//Function used to Take Damage/ Apply Damage to Enemy
void AEnemyAI::Damage(float damageToApply, float pushForce)
{
	//Spawn particles
	if (BloodSplatterFX != nullptr)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatterFX, GetActorLocation());

	//Apply the actual damage
	CurrentHealth -= damageToApply;

	//Call function to start animations and such
	HitEnemy();

	actorLocation = this->GetActorLocation();
	playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector ImpulseDirection = actorLocation - playerLocation;
	GetCharacterMovement()->Velocity += ImpulseDirection.GetSafeNormal() * pushForce;

	//Check if enemy is dead
	if (CurrentHealth <= 0) {

		//LOGIC for dying
		Destroy();
	}
}

// Invoked with the timer to give delay when reaching walkpoints
void AEnemyAI::AIMoveDelay() {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("WALK TRUE"));
	bCanWalk = true;
}

void AEnemyAI::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("DAMAGE PLAYER"));
	}
}

void AEnemyAI::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when Enemy is hit
void AEnemyAI::HitEnemy() {

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("HITTING"));

	AIController->StopMovement();

	bRecentlyHit = true;

	//Perform Animation
	PlayAnimMontage(HitAnimationMontage);
		
}



