// Copyright Epic Games, Inc. All Rights Reserved.

#include "BrickBreaker360GameMode.h"
#include "BrickBreaker360PlayerController.h"
#include "BrickBreaker360Base.h"

ABrickBreaker360GameMode::ABrickBreaker360GameMode()
{
	// no pawn by default
	DefaultPawnClass = ABrickBreaker360Base::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ABrickBreaker360PlayerController::StaticClass();
}
