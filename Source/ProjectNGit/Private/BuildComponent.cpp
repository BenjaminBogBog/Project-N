// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildComponent.h"

// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<APlayerChar>(GetOwner());
	BuildGhost = PlayerRef->BuildPreviewMesh;

	if (PlayerRef != nullptr)
		Camera = PlayerRef->FollowCamera;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't Cast onto PlayerChar: BuildComponent.cpp"));

	// ...
	
}


// Called every frame
void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuildComponent::SpawnBuildGhost()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("BUILDING STATIC MESH"));

	if (BuildGhost != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("SETTING MESH"));
		BuildGhost->SetStaticMesh(MeshToBuild);
		BuildGhost->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BUILD GHOST DOESNT EXIST"));
	}
	

}

void UBuildComponent::GiveBuildColor()
{
	if (BuildGhost != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("GIVING BUILD COLOR"));
		int NumMaterials = BuildGhost->GetNumMaterials();

		for (int i = 0; i < NumMaterials; i++) {
			if (CanBuild) {
				BuildGhost->SetMaterial(i, GreenColor);
			}
			else {
				BuildGhost->SetMaterial(i, RedColor);
			}
		}

		BuildGhost->SetWorldTransform(BuildTransform);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BUILD GHOST DOESNT EXIST"));
	}
	

}

void UBuildComponent::BuildCycle()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("START BUILD CYCLE"));
	FVector TraceStart = Camera->GetComponentLocation() + Camera->GetForwardVector() * 350;
	FVector TraceEnd = Camera->GetComponentLocation() + Camera->GetForwardVector() * 1000;

	FHitResult hit;

	if (GetWorld()->LineTraceSingleByChannel(hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility)) {

		BuildTransform.SetLocation(hit.Location);
		CanBuild = true;
	}
	else {
		BuildTransform.SetLocation(hit.TraceEnd);
		CanBuild = false;
	}

	SpawnBuildGhost();
	GiveBuildColor();
}

