// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_Npc_GamePickUpComponent.h"

UUE5_Npc_GamePickUpComponent::UUE5_Npc_GamePickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UUE5_Npc_GamePickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UUE5_Npc_GamePickUpComponent::OnSphereBeginOverlap);
}

void UUE5_Npc_GamePickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AUE5_Npc_GameCharacter* Character = Cast<AUE5_Npc_GameCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
