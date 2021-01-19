// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpBase.h"
#include "BrickBreaker360Block.h"

// Sets default values
APowerUpBase::APowerUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
	CubeMesh->SetupAttachment(RootComponent);

	Velocity = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void APowerUpBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &APowerUpBase::OnOverlapBegin);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Called every frame
void APowerUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(Velocity * DeltaTime);
}

void APowerUpBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->GetFName().GetPlainNameString().Contains("base"))
	{
		ActivatePowerUp();
		SetActorHiddenInGame(true);
	}
}



void APowerUpBase::StartPowerUp(FVector velocity)
{
	Velocity = velocity;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APowerUpBase::ScaleX(float scale)
{
	SetActorScale3D(FVector(GetActorScale3D().X, scale, GetActorScale3D().Z));
}

void APowerUpBase::ActivatePowerUp_Implementation()
{
	GetWorldTimerManager().SetTimer(PowerUpTimer, this, &APowerUpBase::EndPowerUp, 10.0f, true);
}

void APowerUpBase::EndPowerUp_Implementation()
{
	GetWorldTimerManager().ClearTimer(PowerUpTimer);
	Destroy();
}