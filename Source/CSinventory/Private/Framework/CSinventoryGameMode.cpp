// Copyright Epic Games, Inc. All Rights Reserved.


#include "CSinventory/Public/Framework/CSinventoryGameMode.h"
#include "CSinventory/Public/Player/CSinventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACSinventoryGameMode::ACSinventoryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
