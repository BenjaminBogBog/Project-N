// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerChar.h"

#include "Perception/PawnSensingComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

#include "EnemyAI.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Pushing UMETA(DisplayName = "Pushing"),
	Attack UMETA(DisplayName = "Attack"),
};

UCLASS()
class PROJECTNGIT_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

	//Pawn Sense Component which senses pawns, mostly used to sense players in this case
	UPROPERTY(Category = "AI Settings", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnSensingComponent* pawnSense;

	//Particle system for blood splatter
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNiagaraSystem* BloodSplatterFX;

	//Max Health of the Enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float MaxHealth;

	//Current Health of the Enemy, assign to maxHealth on starting the game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float CurrentHealth;

	//The Index of the Walkpoint in the walkpoint Array
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	int WalkPointIndex;

	//An array of Actors acting as walkpoints
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		TArray<AActor*> WalkPointsActor;

	//The distance to the target before stopping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float AIStopDistance;

	//Stores an enum of the Current AI State
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		EAIState currentAIState;

	//The APawn of the last seen player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	APawn* LastSeen;

	//Speed while walking/patrolling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float WalkSpeed;

	//Speed while running
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float SprintSpeed;

	//Animation Montage of the Enemy attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		UAnimMontage* AttackAnimationMontage;

	//Animation Montage of the Enemy getting hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		UAnimMontage* HitAnimationMontage;

	//Bool to store if the Enemy is attacking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
		bool bIsAttacking;

	//Bool to store if the Enemy is attacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float AIDamage;

	//AI Check Interval
	float intervalTime;

	//Bools which keeps track of states
	bool bCanApplyDamage;
	bool bRecentlyHit;
	bool bCanWalk;
	bool bWalkBoolDebounce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
		bool bSeesPlayer;

	//A Class assigned in BeginPlay() to reference the AI Controller
	AAIController* AIController;

	//Function called when enemy is hit
	void HitEnemy();

	//Function to apply damage to Enemy
	void Damage(float damageToApply, float pushForce = 1000.0f);

	//Function used to bind to delegate OnSeePawn from UPawnSensingComponent which triggers when seeing a pawn
	UFUNCTION()
		void OnSeePawn(APawn* OtherPawn);

	//Function called to give delay to walkpoints
	UFUNCTION()
		void AIMoveDelay();

	//Begin Overlap Function which binds to the delegate
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//End Overlap Function which binds to the delegate
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AttackOnOverlap(AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
