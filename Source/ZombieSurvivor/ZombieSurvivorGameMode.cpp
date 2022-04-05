// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivorGameMode.h"
#include "ZombieSurvivorPlayerController.h"
#include "ZombieSurvivorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZombieSurvivorGameMode::AZombieSurvivorGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AZombieSurvivorPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}