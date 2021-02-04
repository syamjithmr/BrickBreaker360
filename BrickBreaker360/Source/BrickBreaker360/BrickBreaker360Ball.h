// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickBreaker360Ball.generated.h"

UCLASS()
class BRICKBREAKER360_API ABrickBreaker360Ball : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABrickBreaker360Ball();

	// Ball can attach to Base or not.
	UPROPERTY(BlueprintReadWrite)
		bool IsSticky;
	// Ball is attached to Base or not.
	UPROPERTY(BlueprintReadonly)
		bool IsAttached;
	// Number of blocks currently present in the Grid.
	UPROPERTY(BlueprintReadWrite)
		int NoOfBlocks;

	// Shoot the ball in the specified direction when shoot input is given.
	void ShootBall(FVector shootDir);

	// Called when the ball hits something.
	UFUNCTION()
		void OnHit(class AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Direction and Velocity the Ball is currently moving.
	FVector MoveDir;
	float Velocity;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BallMesh;
	UPROPERTY(EditAnywhere)
		class USphereComponent* SphereCollider;
	UPROPERTY(EditAnywhere)
		UPhysicalMaterial* BallPhysMat;

	// Prepare the ball to be attached to the base.
	UFUNCTION(BlueprintCallable)
		void PrepareBallToBeAttached();
	// Called when the game is won.
	UFUNCTION(BlueprintNativeEvent)
		void GameWon();
	void GameWon_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
