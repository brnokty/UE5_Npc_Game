// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"
#include "InteractableInterface.h"
#include "UE5_Npc_GameCharacter.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCCharacter::Interact_Implementation(APlayerController* PlayerController)
{
	// Print a message to the screen when interacted with
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Interacted with NPC!"));
}

void ANPCCharacter::ChooseAction(int choise)
{
    // Aşağıdaki switch-case bloğu, aksiyonları seçmek için kullanılan bir yapı.
    switch (choise)
    {
    case 0:
        // Case 0 işlemleri
        break;
    case 1:
        // Case 1 işlemleri
        break;
    case 2:
        // Case 2 işlemleri
        break;
    default:
        // Default işlemleri
        break;
    }

    Character.EnableInputAgain();
   
            
      
}

