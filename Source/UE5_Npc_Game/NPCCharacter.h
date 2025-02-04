// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractableInterface.h"
#include "UE5_Npc_GameCharacter.h"
#include "NPCCharacter.generated.h"

UCLASS()
class UE5_NPC_GAME_API ANPCCharacter : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact_Implementation(APlayerController* PlayerController) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC") // Blueprint'te düzenlenebilir ve okunabilir
		FString Name;

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void ChooseAction(int choise);

	AUE5_Npc_GameCharacter Character;
	
};
