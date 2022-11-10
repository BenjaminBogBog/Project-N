// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttack_AnimNotifyState.h"

void UPlayerAttack_AnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	actor = MeshComp->GetOwner();
	
	player = Cast<APlayerChar>(actor);

	if (player != nullptr) {
		player->bCanApplyDamage = true;
	}




}

void UPlayerAttack_AnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (player != nullptr) {
		player->bCanApplyDamage = false;
	}
}
