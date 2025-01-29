// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_Npc_GameGameMode.h"
#include "UE5_Npc_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5_Npc_GameGameMode::AUE5_Npc_GameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
