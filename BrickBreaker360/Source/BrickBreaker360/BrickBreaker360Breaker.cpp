// Fill out your copyright notice in the Description page of Project Settings.

#include "BrickBreaker360Breaker.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABrickBreaker360Breaker::ABrickBreaker360Breaker()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BreakerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BreakerMesh"));
	BreakerMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BreakerMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	BreakerMesh->SetStaticMesh(BreakerMeshAsset.Object);
	BreakerMesh->SetRelativeLocation(FVector(0, 0, 0));

	IsSticky = true;
}

ABrickBreaker360Breaker::ABrickBreaker360Breaker(FVector InitialPosition)
{
	ABrickBreaker360Breaker();
	SetActorLocation(InitialPosition);
}

// Called when the game starts or when spawned
void ABrickBreaker360Breaker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrickBreaker360Breaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABrickBreaker360Breaker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

