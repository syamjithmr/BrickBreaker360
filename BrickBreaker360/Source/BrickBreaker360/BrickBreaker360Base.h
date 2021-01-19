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

	/** Pointer to white material used on the focused block */
	UPROPERTY()
		class UMaterialInstance* BaseNotHitMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveBase(float moveRate);
	void ShootBall();
	void AttachBallToBase();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABrickBreaker360Ball> BallObjectClass;
	ABrickBreaker360Ball* BallObject;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int MovementSensitivity;
	int MovementDirection;//1 for Clockwise, -1 for anticlockwise
	float BallAttachPos;
	FVector BaseExtent, BallExtent;
	UPROPERTY(BlueprintReadOnly)
		FRotator Rotation;

};
