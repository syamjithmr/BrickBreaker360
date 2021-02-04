// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBreaker360Ball.h"
#include "BrickBreaker360Block.h"
#include "BrickBreaker360Base.h"
#include "PowerUpBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABrickBreaker360Ball::ABrickBreaker360Ball()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider Component"));
	RootComponent = SphereCollider;
	SphereCollider->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BreakerMesh"));
	BallMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	BallMesh->SetStaticMesh(BallMeshAsset.Object);
	BallMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	BallMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	IsSticky = true;
	IsAttached = true;
	MoveDir = FVector::ZeroVector;
	Velocity = 1000.f;
}

// Called when the game starts or when spawned
void ABrickBreaker360Ball::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &ABrickBreaker360Ball::OnHit);
}

// Called every frame
void ABrickBreaker360Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keep the ball moving in costant velocity.
	FVector currVelocity = SphereCollider->GetPhysicsLinearVelocity();
	if (!IsSticky && !IsAttached && currVelocity.Size() != Velocity)
	{
		if (currVelocity.Normalize())
		{
			SphereCollider->SetPhysicsLinearVelocity(currVelocity * Velocity);
		}
	}
}



void ABrickBreaker360Ball::ShootBall(FVector shootDir)
{
	IsSticky = false;
	IsAttached = false;
	SphereCollider->SetConstraintMode(EDOFMode::XYPlane);
	SphereCollider->SetLinearDamping(0.f);
	SphereCollider->SetPhysMaterialOverride(BallPhysMat);
	SphereCollider->SetCollisionProfileName(TEXT("BlockAll"));
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SphereCollider->SetSimulatePhysics(true);
	SphereCollider->SetPhysicsLinearVelocity(shootDir * Velocity);
	MoveDir = shootDir;
}

void ABrickBreaker360Ball::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsSticky && IsAttached) return;

	if (OtherActor->IsA(ABrickBreaker360Base::StaticClass()))
	{
		// If Ball is sticky and hit Base, attach it to the Base.
		if (IsSticky)
		{
			IsAttached = true;
			ABrickBreaker360Base* base = Cast<ABrickBreaker360Base>(OtherActor);
			FVector hitPointBasePos = Hit.Location - Hit.Normal * 2;
			hitPointBasePos += base->GetActorForwardVector() * base->BaseExtent.X * -1;
			FVector baseToHitPoint = hitPointBasePos - base->GetActorLocation();
			if(FVector::DotProduct(baseToHitPoint, FVector::CrossProduct(base->GetActorForwardVector(), FVector::UpVector))>0)
				base->BallAttachPos = baseToHitPoint.Size();
			else
				base->BallAttachPos = -baseToHitPoint.Size();
			SphereCollider->SetSimulatePhysics(false);
			SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		// If the ball hits a Block, spawn PowerUp if one is avilable and destroy the block.
		if (OtherActor->IsA(ABrickBreaker360Block::StaticClass()))
		{
			if (APowerUpBase* powerUp = Cast<ABrickBreaker360Block>(OtherActor)->PowerUp)
			{
				FVector PowerUpVelocity = SphereCollider->GetPhysicsLinearVelocity();
				powerUp->StartPowerUp(PowerUpVelocity.GetSafeNormal());
			}
			OtherActor->Destroy();
			// If no blocks are present, win the game.
			if (--NoOfBlocks < 1)
				GameWon();
		}
	}
}

void ABrickBreaker360Ball::PrepareBallToBeAttached()
{
	IsSticky = true;
	IsAttached = true;
	
	SphereCollider->SetSimulatePhysics(false);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABrickBreaker360Ball::GameWon_Implementation()
{
	SphereCollider->SetSimulatePhysics(false);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
