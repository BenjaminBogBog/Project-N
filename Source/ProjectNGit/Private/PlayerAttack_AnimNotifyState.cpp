// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttack_AnimNotifyState.h"
#include "PlayerChar.h"
#include "EnemyAI.h"

void UPlayerAttack_AnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	actor = MeshComp->GetOwner();
	
	player = Cast<APlayerChar>(actor);
	enemy = Cast<AEnemyAI>(actor);

	if (player != nullptr) {
		player->bCanApplyDamage = true;
	}

	if (enemy != nullptr) {
		enemy->bCanApplyDamage = true;
	}

}

void UPlayerAttack_AnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (player != nullptr) {
		player->bCanApplyDamage = false;
	}

	if (enemy != nullptr) {
		enemy->bCanApplyDamage = false;
	}
}
