// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BrickBreaker360Breaker.generated.h"

UCLASS()
class BRICKBREAKER360_API ABrickBreaker360Breaker : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABrickBreaker360Breaker();
	ABrickBreaker360Breaker(FVector InitialPosition);

	bool IsSticky;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* DummyRoot;
	UPROPERTY(EditAnyWhere)
	UStaticMeshComponent* BreakerMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
