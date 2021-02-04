// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpBase.generated.h"

UCLASS()
class BRICKBREAKER360_API APowerUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUpBase();

	// Decides if the PowerUp has a timer.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool HasTimer;
	// If PowerUp HasTimer, represent the time remaining. Else represents the number of PowerUps available.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TimeRemaining;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* CubeMesh;
	FVector Velocity;
	FTimerHandle PowerUpTimer;
	class UBrickBreaker360HUD_UI_Base* HUD_UI;
	// Used to get image displayed in the PowerUPUI.
	UPROPERTY(BlueprintReadOnly)
		class UMaterialInstanceDynamic* MaterialInstance;
	// Used to remove the PowerUpUISlot when calling PowerUpDeactivated() in HUD class.
	UPROPERTY(BlueprintReadWrite)
		class UUserWidget* PowerUPUISlot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to enable and move the PowerUp in a specific direction.
	void StartPowerUp(FVector velocity);
	// Called to activate the PowerUp.
	UFUNCTION(BlueprintNativeEvent)
		void ActivatePowerUp();
	virtual void ActivatePowerUp_Implementation();
	UFUNCTION(BlueprintCallable)
		void UpdatePowerUpTimer();
	// Called to deactivate the PowerUp.
	UFUNCTION(BlueprintNativeEvent)
		void EndPowerUp();
	virtual void EndPowerUp_Implementation();
	// Scale the PowerUp.
	void ScaleX(float scale);
	void ScaleXY(float scale);
};
