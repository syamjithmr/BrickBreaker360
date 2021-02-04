// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpBase.h"
#include "BrickBreaker360Block.h"
#include "BrickBreaker360Base.h"
#include "BrickBreaker360HUD_UI_Base.h"
#include "BrickBreaker360GameMode.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APowerUpBase::APowerUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/Meshes/PuzzleCube.PuzzleCube"));
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
	CubeMesh->SetupAttachment(RootComponent);

	Velocity = FVector::ZeroVector;
	HasTimer = true;
	TimeRemaining = 5.f;
}

// Called when the game starts or when spawned
void APowerUpBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &APowerUpBase::OnOverlapBegin);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	MaterialInstance = CubeMesh->CreateDynamicMaterialInstance(0, CubeMesh->GetMaterial(0));
}

// Called every frame
void APowerUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(Velocity * DeltaTime);
}

void APowerUpBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(ABrickBreaker360Base::StaticClass()))
	{
		// Activate the power up on interacting with Base.
		HUD_UI = Cast<ABrickBreaker360Base>(OtherActor)->HUD_UI;
		ActivatePowerUp();
	}
}



void APowerUpBase::StartPowerUp(FVector velocity)
{
	// Show and add velocity.
	Velocity = velocity * 500;
	SetActorHiddenInGame(false);
	// Enable to collision.
	SetActorEnableCollision(true);
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APowerUpBase::ScaleX(float scale)
{
	SetActorScale3D(FVector(GetActorScale3D().X, scale, GetActorScale3D().Z));
}

void APowerUpBase::ScaleXY(float scale)
{
	SetActorScale3D(FVector(scale, scale, GetActorScale3D().Z));
}

void APowerUpBase::ActivatePowerUp_Implementation()
{
	// Hide and stop the PowerUp, and disable collision.
	SetActorHiddenInGame(true);
	CubeMesh->SetGenerateOverlapEvents(false);
	Velocity = FVector::ZeroVector;

	ABrickBreaker360GameMode* gameMode = Cast<ABrickBreaker360GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	APowerUpBase* activatedPowerUp = nullptr;
	for (APowerUpBase* tempPowerUp : gameMode->ActivatedPowerUps)
		if (this->IsA(tempPowerUp->GetClass()))
		{
			activatedPowerUp = tempPowerUp;
			break;
		}
	if (activatedPowerUp)
	{
		// If this PowerUp is already activated, Update TimeRemaining of the existing PowerUp.
		if (activatedPowerUp->HasTimer)
			activatedPowerUp->TimeRemaining += TimeRemaining;
		else
			activatedPowerUp->TimeRemaining++;
	}
	else
	{
		// If this PowerUp is not activated already, start the PowerUp and add it to the array.
		if (HasTimer)
			GetWorldTimerManager().SetTimer(PowerUpTimer, this, &APowerUpBase::UpdatePowerUpTimer, 1.f, true);
		else
			TimeRemaining = 1.f;
		gameMode->ActivatedPowerUps.Add(this);
		// Update HUD.
		HUD_UI->PowerUpActivated(this);
	}
}

void APowerUpBase::UpdatePowerUpTimer()
{
	TimeRemaining--;
	if (TimeRemaining < 0)
		EndPowerUp();
}

void APowerUpBase::EndPowerUp_Implementation()
{
	// Remove the PowerUp from the array.
	ABrickBreaker360GameMode* gameMode = Cast<ABrickBreaker360GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	for (APowerUpBase* tempPowerUp : gameMode->ActivatedPowerUps)
		if (this->IsA(tempPowerUp->GetClass()))
		{
			gameMode->ActivatedPowerUps.Remove(tempPowerUp);
			break;
		}
	GetWorldTimerManager().ClearTimer(PowerUpTimer);
	// Update HUD.
	HUD_UI->PowerUpDeactivated(this);
	Destroy();
}

