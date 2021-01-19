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
	/** Number of rows of grid **/
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
		int Rows;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int Radius;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABrickBreaker360Block> BrickBreaker360BlockClass;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	TArray<class ABrickBreaker360Block*> BlockArray;

	float RotationSpeed;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	UPROPERTY(BlueprintReadwrite)
		bool CanRotate;
};



