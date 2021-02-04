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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TimeRemaining;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool HasTimer;

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
	UPROPERTY(BlueprintReadOnly)
		class UMaterialInstanceDynamic* MaterialInstance;
	UPROPERTY(BlueprintReadWrite)
		class UUserWidget* PowerUPUISlot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartPowerUp(FVector velocity);
	UFUNCTION(BlueprintNativeEvent)
		void ActivatePowerUp();
	virtual void ActivatePowerUp_Implementation();
	UFUNCTION(BlueprintCallable)
		void UpdatePowerUpTimer();
	UFUNCTION(BlueprintNativeEvent)
		void EndPowerUp();
	virtual void EndPowerUp_Implementation();
	void ScaleX(float scale);
	void ScaleXY(float scale);
};
