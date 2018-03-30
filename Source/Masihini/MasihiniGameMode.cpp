// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MasihiniGameMode.h"
#include "Rover.h"

AMasihiniGameMode::AMasihiniGameMode()
{
	DefaultPawnClass = ARover::StaticClass();
}
