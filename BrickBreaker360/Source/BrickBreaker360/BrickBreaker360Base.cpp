// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBreaker360Base.h"
#include "BrickBreaker360Ball.h"
#include "BrickBreaker360BlockGrid.h"
#include "BrickBreaker360HUD_UI_Base.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABrickBreaker360Base::ABrickBreaker360Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> BasePlainMesh(TEXT("/Game/Meshes/PuzzleCube.PuzzleCube"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> BaseNotHitMat(TEXT("MaterialInstanceConstant'/Game/Materials/BaseMaterialNotHit.BaseMaterialNotHit'"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh0"));
	BaseMesh->SetStaticMesh(BasePlainMesh.Object);
	BaseMesh->SetMaterial(0, BaseNotHitMat.Object);
	BaseMesh->SetupAttachment(RootComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(BaseMesh);
	BoxCollider->SetWorldScale3D(FVector(3.98f, 3.98f, 3.98f));

	RootComponent->SetWorldScale3D(FVector(0.25f, 2.f, 0.25f));
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	BaseNotHitMaterial = BaseNotHitMat.Object;
	MovementSensitivity = 2;
	MovementDirection = -1;
	BallAttachPos = 0.f;
	Rotation = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void ABrickBreaker360Base::BeginPlay()
{
	Super::BeginPlay();

	RootComponent->SetWorldLocation(FVector(-1400.f, 0.f, 25.f));

	FVector temp;
	GetActorBounds(false, temp, BaseExtent);

	if(BallObjectClass)
		BallObject = GetWorld()->SpawnActor<ABrickBreaker360Ball>(BallObjectClass);
	BallObject->GetActorBounds(false, temp, BallExtent);
	AttachBallToBase();
	ABrickBreaker360BlockGrid* grid = Cast<ABrickBreaker360BlockGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), ABrickBreaker360BlockGrid::StaticClass()));
	if(grid)
		BallObject->NoOfBlocks = grid->BlockArray.Num();

	
	if (HUD_UI_Class)
	{
		HUD_UI = Cast<UBrickBreaker360HUD_UI_Base>(CreateWidget(GetWorld(), HUD_UI_Class));
		HUD_UI->AddToViewport();
	}
}

void ABrickBreaker360Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveBase", this, &ABrickBreaker360Base::MoveBase);
	PlayerInputComponent->BindAction("ShootBall", IE_Pressed, this, &ABrickBreaker360Base::ShootBall);
}

// Called every frame
void ABrickBreaker360Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABrickBreaker360Base::MoveBase(float moveRate)
{
	if (moveRate != 0)
	{
		FVector currentLocation = GetActorLocation();
		currentLocation = currentLocation.RotateAngleAxis(moveRate * MovementDirection * MovementSensitivity, FVector(0.f, 0.f, 1.0f));
		SetActorLocation(currentLocation);
		FRotator toRotate = FRotator(0.f, moveRate * MovementDirection * MovementSensitivity, 0.f);
		AddActorWorldRotation(toRotate);
		Rotation = toRotate;
		if (BallObject->IsSticky && BallObject->IsAttached)
		{
			AttachBallToBase();
		}
	}
}

void ABrickBreaker360Base::ShootBall()
{
	if(BallObject->IsAttached)
		BallObject->StartBall(GetActorForwardVector());
}

void ABrickBreaker360Base::AttachBallToBase()
{
	FHitResult hitRes;
	FVector attachPointRef = FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector) * BallAttachPos;
	ActorLineTraceSingle(hitRes, GetActorLocation() + attachPointRef, GetActorForwardVector() * 100, ECollisionChannel::ECC_Camera, FCollisionQueryParams());
	BallObject->SetActorLocation(hitRes.ImpactPoint + GetActorForwardVector() * BallExtent.X * 2);
	BallObject->SetActorLocation(FVector(BallObject->GetActorLocation().X, BallObject->GetActorLocation().Y, GetActorLocation().Z));
}
