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

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
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
	if (OtherActor->GetFName().GetPlainNameString().Contains("base"))
	{
		HUD_UI = Cast<ABrickBreaker360Base>(OtherActor)->HUD_UI;
		ActivatePowerUp();
	}
}



void APowerUpBase::StartPowerUp(FVector velocity)
{
	Velocity = velocity * 500;
	SetActorHiddenInGame(false);
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
		if (activatedPowerUp->HasTimer)
			activatedPowerUp->TimeRemaining += TimeRemaining;
	}
	else
	{
		if (HasTimer)
			GetWorldTimerManager().SetTimer(PowerUpTimer, this, &APowerUpBase::UpdatePowerUpTimer, 1.f, true);
		gameMode->ActivatedPowerUps.Add(this);
		HUD_UI->PowerUpActivated(this);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Activated Power Ups: %d"), gameMode->ActivatedPowerUps.Num())
}

void APowerUpBase::UpdatePowerUpTimer()
{
	TimeRemaining--;
	if (TimeRemaining < 0)
		EndPowerUp();
}

void APowerUpBase::EndPowerUp_Implementation()
{
	ABrickBreaker360GameMode* gameMode = Cast<ABrickBreaker360GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	for (APowerUpBase* tempPowerUp : gameMode->ActivatedPowerUps)
		if (this->IsA(tempPowerUp->GetClass()))
		{
			gameMode->ActivatedPowerUps.Remove(tempPowerUp);
			break;
		}
	//UE_LOG(LogTemp, Warning, TEXT("Activated Power Ups: %d"), gameMode->ActivatedPowerUps.Num())
	GetWorldTimerManager().ClearTimer(PowerUpTimer);
	HUD_UI->PowerUpDeactivated(this);
	Destroy();
}

