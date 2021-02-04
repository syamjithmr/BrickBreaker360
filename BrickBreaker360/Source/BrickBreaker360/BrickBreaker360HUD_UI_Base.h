// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickBreaker360HUD_UI_Base.generated.h"

/**
 * Represents HUD
 */
UCLASS()
class BRICKBREAKER360_API UBrickBreaker360HUD_UI_Base : public UUserWidget
{
	GENERATED_BODY()

public:
	// Called when a PowerUp is activated. Defined in the derived Widget.
	UFUNCTION(BlueprintNativeEvent)
		void PowerUpActivated(class APowerUpBase* powerUp);
	virtual void PowerUpActivated_Implementation(APowerUpBase* powerUp);
	// Called when a PowerUp is deactivated. Defined in the derived Widget.
	UFUNCTION(BlueprintNativeEvent)
		void PowerUpDeactivated(APowerUpBase* powerUp);
	virtual void PowerUpDeactivated_Implementation(APowerUpBase* powerUp);
};
