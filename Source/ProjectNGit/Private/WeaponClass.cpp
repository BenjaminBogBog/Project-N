// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponClass.h"

// Sets default values
AWeaponClass::AWeaponClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

