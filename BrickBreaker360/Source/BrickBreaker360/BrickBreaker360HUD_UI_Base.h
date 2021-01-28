// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickBreaker360HUD_UI_Base.generated.h"

/**
 * 
 */
UCLASS()
class BRICKBREAKER360_API UBrickBreaker360HUD_UI_Base : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		void PowerUpActivated(class APowerUpBase* powerUp);
	virtual void PowerUpActivated_Implementation(APowerUpBase* powerUp);
	UFUNCTION(BlueprintNativeEvent)
		void PowerUpDeactivated(APowerUpBase* powerUp);
	virtual void PowerUpDeactivated_Implementation(APowerUpBase* powerUp);
};
