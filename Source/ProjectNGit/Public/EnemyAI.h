// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Perception/PawnSensingComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include <string>
#include "AIController.h"

#include "EnemyAI.generated.h"

UCLASS()
class PROJECTNGIT_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

	UPawnSensingComponent* pawnSense;

	UFUNCTION()
		void AIMoveDelay();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNiagaraSystem* BloodSplatterFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentHealth;

	void ApplyDamage(float damageToApply);

	int WalkPointIndex;

	bool bCanWalk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		TArray<AActor*> WalkPointsActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
		float AIStopDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
