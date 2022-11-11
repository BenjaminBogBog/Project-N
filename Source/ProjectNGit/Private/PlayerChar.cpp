// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"
#include "EnemyAI.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetOffset = FVector(0, 0, 50.0);
	CameraBoom->TargetArmLength = 450.0;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetFieldOfView(90.0);
	FollowCamera->bUsePawnControlRotation = false;

	bDead = false;

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* hitboxComp = GetMesh()->GetChildComponent(0);
	UBoxComponent* hitbox = Cast<UBoxComponent>(hitboxComp);

	if (hitbox != nullptr)
		hitbox->OnComponentBeginOverlap.AddDynamic(this, &APlayerChar::OnBeginOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find box Component"));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bCanAttack = true;

	CurrentHealth = MaxHealth;
	

	CheckPoint = GetActorLocation();
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling()) {
		fallTimer += DeltaTime;
	}

	if (GetCharacterMovement()->IsMovingOnGround()) {
		if (fallTimer >= 1.75f) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("DIE FROM FALL DAMAGE"));

			//Die
			SetActorLocation(CheckPoint);
		}

		fallTimer = 0;
	}
}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerChar::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerChar::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerChar::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &APlayerChar::Attack);

}

void APlayerChar::MoveForward(float Axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Axis);
	}
}

void APlayerChar::MoveRight(float Axis)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Axis);
}

void APlayerChar::InitAttackCooldown()
{
	bCanAttack = true;
}

void APlayerChar::StartSprint() {

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerChar::StopSprint() {

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerChar::Attack() {

	if (bCanAttack && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) {

		switch (comboProgression) {
		case 0:
			PlayAnimMontage(attackLightAnim);
			comboProgression = 1;
			AttackRate = 0.001f;
			break;
		case 1:
			PlayAnimMontage(attackHeavyAnim);
			comboProgression = 0;
			AttackRate = 0.7f;
			break;
		}
			

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerChar::InitAttackCooldown, AttackRate, false);

		bCanAttack = false;
	}
		

	
}

void APlayerChar::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AttackOnOverlap(OtherActor);

}

void APlayerChar::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackOnOverlap(OtherActor);
}

void APlayerChar::AttackOnOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Enemy") && bCanApplyDamage) {

		ACharacter* enemyCharacter = Cast<ACharacter>(OtherActor);

		AEnemyAI* enemy = Cast<AEnemyAI>(enemyCharacter);

		if (enemy != nullptr)
			enemy->Damage(CurrentDamage);

		bCanApplyDamage = false;
	}
}

void APlayerChar::Damage(float damage, AActor* OtherActor, float pushForce) {
	//Spawn particles
	if (BloodSplatterFX != nullptr)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatterFX, GetActorLocation());

	//Apply the actual damage
	CurrentHealth -= damage;

	//Call function to start animations and such
	PlayerHit();

	FVector actorLocation = this->GetActorLocation();
	FVector playerLocation = OtherActor->GetActorLocation();
	FVector ImpulseDirection = actorLocation - playerLocation;
	GetCharacterMovement()->Velocity += ImpulseDirection.GetSafeNormal() * pushForce;

	//Check if enemy is dead
	if (CurrentHealth <= 0) {

		//LOGIC for dying
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("U DEAD MAN"));

		Destroy();
	}
}

void APlayerChar::PlayerHit() {
	bRecentlyHit = true;

	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(attackHeavyAnim) || GetMesh()->GetAnimInstance()->Montage_IsPlaying(attackLightAnim)) {
		GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
		bCanApplyDamage = false;
	}

	//Perform Animation
	PlayAnimMontage(HitAnim);
}

