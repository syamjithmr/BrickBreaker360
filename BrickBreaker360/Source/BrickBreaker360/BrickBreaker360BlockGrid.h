// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickBreaker360BlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ABrickBreaker360BlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

public:
	ABrickBreaker360BlockGrid();
	// Number of rows of grid.
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
		int Rows;
	// Radius of the Grid.
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int Radius;
	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
		float BlockSpacing;
	// If the Grid can rotate.
	UPROPERTY(BlueprintReadwrite)
		bool CanRotate;
	int NoOfBlocks;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABrickBreaker360Block> BrickBreaker360BlockClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float RotationSpeed;

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

};



