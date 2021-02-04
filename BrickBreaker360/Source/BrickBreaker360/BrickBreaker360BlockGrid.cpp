// Copyright Epic Games, Inc. All Rights Reserved.

#include "BrickBreaker360BlockGrid.h"
#include "BrickBreaker360Block.h"
#include "PowerUpBase.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ABrickBreaker360BlockGrid::ABrickBreaker360BlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	SetActorLocation(FVector::ZeroVector);

	// Set defaults
	Radius = 1000;
	Rows = 5;
	BlockSpacing = 20.f;
	NoOfBlocks = 0;

	RotationSpeed = 0.f;
	CanRotate = false;
}


void ABrickBreaker360BlockGrid::BeginPlay()
{
	Super::BeginPlay();
	float rowHeight = 2 * Radius / Rows;

	ABrickBreaker360Block* SampleBlock = GetWorld()->SpawnActor<ABrickBreaker360Block>(BrickBreaker360BlockClass, FVector::ZeroVector, FRotator::ZeroRotator);
	FVector blockOrigin, blockExtent;
	SampleBlock->GetActorBounds(false, blockOrigin, blockExtent);
	if (SampleBlock->PowerUp)
		SampleBlock->PowerUp->Destroy();
	SampleBlock->Destroy();
	float blockWidth = blockExtent.Y * 2, blockHeight = blockExtent.X * 2;

	// Loop to spawn each block
	for(int rowIndex=0; rowIndex<Rows; rowIndex++)
	{
		float rowMidX = Radius - (rowIndex + 1) * rowHeight + rowHeight/2;
		//Find start and end y position of row, assuming grid is centered at (0,0,0).
		float c = rowMidX * rowMidX - Radius * Radius;
		float leftPointY = UKismetMathLibrary::Sqrt(-4 * c) / 2;
		float rightPointY = -1 * UKismetMathLibrary::Sqrt(-4 * c) / 2;
		if (leftPointY > rightPointY)
			Swap(leftPointY, rightPointY);
		float rowWidth = rightPointY - leftPointY;

		while(rowWidth>0.f)
		{
			float maxWidth = rowWidth / 2;
			if (maxWidth < rowHeight)
				maxWidth = rowHeight;
			float currBlockWidth = UKismetMathLibrary::RandomFloatInRange(rowHeight, maxWidth);
			if (rowWidth < rowHeight * 2)
				currBlockWidth = rowWidth;
			float XOffset = rowMidX;
			float YOffset = leftPointY+currBlockWidth/2;
			leftPointY += currBlockWidth;
			rowWidth -= currBlockWidth;

			// Make position vector, offset from Grid location
			FVector BlockLocation = FVector(XOffset, YOffset, 0.f);

			// Spawn a block
			ABrickBreaker360Block* NewBlock = GetWorld()->SpawnActor<ABrickBreaker360Block>(BrickBreaker360BlockClass, BlockLocation, FRotator(0, 0, 0));
			NewBlock->SetActorScale3D(FVector((rowHeight - BlockSpacing) / blockHeight, (currBlockWidth - BlockSpacing) / blockWidth, NewBlock->GetActorScale3D().Z));
			NewBlock->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			NewBlock->SetPowerUpScale(((float)Radius / 5) / blockExtent.X);
			NoOfBlocks++;
		}
	}
}

#undef LOCTEXT_NAMESPACE
