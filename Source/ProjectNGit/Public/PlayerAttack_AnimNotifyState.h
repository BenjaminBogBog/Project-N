// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerChar.h"
#include "PlayerAttack_AnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNGIT_API UPlayerAttack_AnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	AActor* actor;
	APlayerChar* player;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
