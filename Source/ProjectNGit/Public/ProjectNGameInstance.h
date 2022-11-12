// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WeaponClass.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectNGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNGIT_API UProjectNGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Settings")
		TArray<FString> WeaponNames;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Game Settings")
		TMap<FString, TSubclassOf<AWeaponClass>> WeaponBlueprints;
	
};
