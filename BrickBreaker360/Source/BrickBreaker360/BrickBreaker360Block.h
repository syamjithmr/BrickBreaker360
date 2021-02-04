// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickBreaker360Block.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class ABrickBreaker360Block : public AActor
{
	GENERATED_BODY()
	
	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ABrickBreaker360Block();

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks(Blocks with a power Up) */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	// Array containing all Spawnable Power Ups.
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class APowerUpBase>> PowerUpArray;
	// Power Up attached to this Block. Null if no Power Up is attached.
	UPROPERTY(VisibleAnywhere)
		APowerUpBase* PowerUp;
	// Probability of a Power Up to spawn.
	UPROPERTY(EditAnywhere)
		int PowerUpSpawnProbability;
	// Set the scale of Power Up.
	void SetPowerUpScale();
	void SetPowerUpScale(float ScaleXY);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};
