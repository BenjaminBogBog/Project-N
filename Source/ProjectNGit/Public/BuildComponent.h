// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerChar.h"
#include "BuildComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNGIT_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildComponent();

	bool IsBuildModeOn;
	bool CanBuild;

	UStaticMeshComponent* BuildGhost;
	FTransform BuildTransform;
	UCameraComponent* Camera;
	APlayerChar* PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Build Settings")
		UStaticMesh* MeshToBuild;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Build Settings")
		UMaterialInterface* GreenColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Build Settings")
		UMaterialInterface* RedColor;


	void SpawnBuildGhost();
	void GiveBuildColor();
	void BuildCycle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
