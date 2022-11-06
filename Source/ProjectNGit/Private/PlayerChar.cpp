// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"


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
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void APlayerChar::StartSprint() {

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerChar::StopSprint() {

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerChar::Attack() {
	PlayAnimMontage(attackAnim);
}

void APlayerChar::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());

	if (OtherActor->ActorHasTag("Enemy")) {
		OtherActor->Destroy();
	}

}

void APlayerChar::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

