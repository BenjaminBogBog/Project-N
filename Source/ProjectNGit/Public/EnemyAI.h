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

	bool bCanWalk;
	bool bWalkBoolDebounce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	bool bSeesPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		TArray<AActor*> WalkPointsActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float AIStopDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		EAIState currentAIState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Settings")
	APawn* LastSeen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float SprintSpeed;


	//AI Check Interval
	float intervalTime;

	AAIController* AIController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
