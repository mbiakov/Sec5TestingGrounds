// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Sec5TestingGroundsGameMode.h"
#include "Sec5TestingGroundsHUD.h"
#include "Sec5TestingGroundsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASec5TestingGroundsGameMode::ASec5TestingGroundsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASec5TestingGroundsHUD::StaticClass();
}
