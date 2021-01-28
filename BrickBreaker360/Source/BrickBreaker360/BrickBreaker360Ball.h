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

	UPROPERTY(BlueprintReadWrite)
	bool IsSticky;
	bool IsAttached;

	void StartBall(FVector shootDir);

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
	UFUNCTION()
	void OnHit(class AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector MoveDir;
	float Velocity;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere)
		class USphereComponent* SphereCollider;
	
	UPROPERTY(EditAnywhere)
		UPhysicalMaterial* BallPhysMat;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
