// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Perception/PawnSensingComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	UPROPERTY(Category = "AI Settings", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnSensingComponent* pawnSense;

	UFUNCTION()
		void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
		void AIMoveDelay();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNiagaraSystem* BloodSplatterFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentHealth;

	void ApplyDamage(float damageToApply);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	int WalkPointIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	bool bRecentlyHit;
	bool bCanWalk;
	bool bWalkBoolDebounce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	bool bSeesPlayer;

	//Bool to store if the player is attacking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	bool bIsAttacking;

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


	//AI Check Interval
	float intervalTime;

	AAIController* AIController;

	//Function called when enemy is hit
	void HitEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
