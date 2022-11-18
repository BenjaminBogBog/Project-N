// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "Shrine.generated.h"

UCLASS()
class PROJECTNGIT_API AShrine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShrine();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shrine Settings") 
		float PlayerHealthAddedPerSecond;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shrine Settings")
		float DangerDangerDanger;
	
	//Shere Component sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sphere")
	USphereComponent* sphere;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
