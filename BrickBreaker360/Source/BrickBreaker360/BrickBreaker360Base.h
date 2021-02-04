// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BrickBreaker360Base.generated.h"

UCLASS()
class BRICKBREAKER360_API ABrickBreaker360Base : public APawn
{
	GENERATED_BODY()

	/** StaticMesh component for the base */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxCollider;

public:	
	// Sets default values for this actor's properties
	ABrickBreaker360Base();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
		class UMaterialInstance* BaseNotHitMaterial;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UBrickBreaker360HUD_UI_Base> HUD_UI_Class;
	UBrickBreaker360HUD_UI_Base* HUD_UI;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Moves the Base according to movement input.
	void MoveBase(float moveRate);
	// Shoot the ball in Base's forward direction.
	void ShootBall();
	// Attach the ball to Base.
	void AttachBallToBase();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABrickBreaker360Ball> BallObjectClass;
	ABrickBreaker360Ball* BallObject;

public:
	// Decides mmovement direction and sensitivity.
	int MovementSensitivity;
	int MovementDirection;//1 for Clockwise, -1 for anticlockwise.
	// Position to attach the Ball.
	float BallAttachPos;
	// Bounds of Base and Ball object.
	FVector BaseExtent, BallExtent;
	// Current rotation of the Base. Used to rotate the main menu objects.
	UPROPERTY(BlueprintReadOnly)
		FRotator Rotation;

};
