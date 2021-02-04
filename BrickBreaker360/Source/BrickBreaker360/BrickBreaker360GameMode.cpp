// Copyright Epic Games, Inc. All Rights Reserved.

#include "BrickBreaker360GameMode.h"
#include "BrickBreaker360PlayerController.h"
#include "BrickBreaker360Base.h"
#include "PowerUpBase.h"

ABrickBreaker360GameMode::ABrickBreaker360GameMode()
{
	// Base as Default Pawn
	DefaultPawnClass = ABrickBreaker360Base::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ABrickBreaker360PlayerController::StaticClass();
}
