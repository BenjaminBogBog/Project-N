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
#include "ProjectNGameInstance.h"

#include "PlayerChar.generated.h"

class AEnemyAI;
class UBuildComponent;

UENUM(BlueprintType)
enum class EPlayerState : uint8 {
	Friendly UMETA(DisplayName = "Friendly"),
	Combat UMETA(DisplayName = "Combat"),
	Building UMETA(DisplayName = "Building")
};

UCLASS()
class PROJECTNGIT_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
	EPlayerState CurrentPlayerState;

#pragma region Movement and Combat
	//Particle system for blood splatter
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNiagaraSystem* BloodSplatterFX;

	//Function which binds to OnBeginOverlap delegate
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Function which binds to OnEndOverlap delegate
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Attack Functions which calls on both begin and end overlap
	void AttackOnOverlap(AActor* OtherActor);

	//SpringArm Component which acts as the Camera Boom to give distance from the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		USpringArmComponent* CameraBoom;

	//Camera Component attached to the CameraBoom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* FollowCamera;

	//Function to move player forward
	void MoveForward(float Axis);

	//Function to move player to the right
	void MoveRight(float Axis);

	//Applies cooldown to the player after attacking
	UFUNCTION()
	void InitAttackCooldown();

	//Time it takes before player can attack again
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		float AttackRate;

	//The speed of walking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		float WalkSpeed;

	//The speed of sprinting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		float SprintSpeed;

	//First Attack Animation to play
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		UAnimMontage* attackLightAnim;

	//Second Attack Animation to play
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		UAnimMontage* attackHeavyAnim;

	//Hit Animation to play when player is hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
		UAnimMontage* HitAnim;

	//Current Damage of player, inherits from WeaponDamage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float CurrentDamage;

	//Current health of the player, inherits from MaxHealth onBeginPlay
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float CurrentHealth;

	//Max health of the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float MaxHealth;

	//Stores the data of which weapon is equipped
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player States")
		AWeaponClass* equippedWeapon;

	//Bool which states if player can attack
	UPROPERTY(VisibleAnywhere)
		bool bCanAttack;

	//A bunch of bool states used
	FVector CheckPoint;
	bool bDead;
	int comboProgression; //0 = light, 1 = heavy
	float fallTimer;
	bool bRecentlyHit;
	bool bCanApplyDamage;

	int weaponIndex;

#pragma endregion 

#pragma region Building

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Build Settings")
	UBuildComponent* BuildComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Build Settings")
		UStaticMeshComponent* BuildPreviewMesh;

	void DestroyComponent();
	void StartBuilding();

#pragma endregion


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input Functions - Functions that trigger on input
	void StartSprint();
	void StopSprint();

	void Attack();
	void SwitchWeapon();
	void StartLoop();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerHit();

	void Damage(float damage, AActor* OtherActor, float pushForce = 1000.0f);

};
