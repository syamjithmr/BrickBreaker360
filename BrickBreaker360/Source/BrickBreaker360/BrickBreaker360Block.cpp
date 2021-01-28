// Copyright Epic Games, Inc. All Rights Reserved.

#include "BrickBreaker360Block.h"
#include "BrickBreaker360BlockGrid.h"
#include "PowerUpBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Kismet/KismetMathLibrary.h"

ABrickBreaker360Block::ABrickBreaker360Block()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("Material'/Game/Materials/BaseMaterial.BaseMaterial'"))
			, BlueMaterial(TEXT("MaterialInstanceConstant'/Game/Materials/BlueMaterial.BlueMaterial'"))
			, OrangeMaterial(TEXT("MaterialInstanceConstant'/Game/Materials/OrangeMaterial.OrangeMaterial'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
}

// Called when the game starts or when spawned
void ABrickBreaker360Block::BeginPlay()
{
	Super::BeginPlay();

	PowerUp = nullptr;
	int PowerUpIndex = UKismetMathLibrary::RandomIntegerInRange(-1, PowerUpArray.Num() - 1);
	if (PowerUpIndex != -1 && PowerUpArray.Num() != 0)
	{
		UClass* PowerUpClass = PowerUpArray[PowerUpIndex].Get();
		PowerUp = GetWorld()->SpawnActor<APowerUpBase>(PowerUpClass);
		PowerUp->SetActorLocation(FVector(0.f, 0.f, 25.f));
		PowerUp->SetActorScale3D(BlockMesh->GetRelativeScale3D());
		PowerUp->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ABrickBreaker360Block::SetPowerUpScale()
{
	if(PowerUp)
		PowerUp->ScaleX(GetActorScale3D().X);
}

void ABrickBreaker360Block::SetPowerUpScale(float ScaleXY)
{
	if (PowerUp)
		PowerUp->ScaleXY(ScaleXY);
}