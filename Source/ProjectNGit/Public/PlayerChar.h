// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/TransformNonVectorized.h"
#include "Kismet/GameplayStatics.h"

#include "PlayerChar.generated.h"

class AEnemyAI;

UCLASS()
class PROJECTNGIT_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

	//Particle system for blood splatter
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNiagaraSystem* BloodSplatterFX;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AttackOnOverlap(AActor* OtherActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* FollowCamera;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	UFUNCTION()
	void InitAttackCooldown();

	FVector CheckPoint;
	bool bDead;
	int comboProgression; //0 = light, 1 = heavy
	float fallTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		UAnimMontage* attackLightAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		UAnimMontage* attackHeavyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		UAnimMontage* HitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float CurrentDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float MaxHealth;

	UPROPERTY(VisibleAnywhere)
		bool bCanAttack;

	bool bRecentlyHit;
	bool bCanApplyDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input Functions - Functions that trigger on input
	void StartSprint();
	void StopSprint();

	void Attack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerHit();

	void Damage(float damage, AActor* OtherActor, float pushForce = 1000.0f);

};
