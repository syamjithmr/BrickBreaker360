// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBreaker360Ball.h"
#include "BrickBreaker360Block.h"
#include "BrickBreaker360Base.h"
#include "PowerUpBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ABrickBreaker360Ball::ABrickBreaker360Ball()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create dummy root scene component
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider Component"));
	//SphereCollider->SetupAttachment(RootComponent);
	RootComponent = SphereCollider;
	SphereCollider->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BreakerMesh"));
	BallMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	BallMesh->SetStaticMesh(BallMeshAsset.Object);
	BallMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	BallMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));

	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMatAsset(TEXT("PhysicalMaterial'/Game/BallPhyscalMat.BallPhyscalMat'"));
	BallPhysMat = PhysMatAsset.Object;
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

	OnActorBeginOverlap.AddDynamic(this, &ABrickBreaker360Ball::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ABrickBreaker360Ball::OnOverlapEnd);
	OnActorHit.AddDynamic(this, &ABrickBreaker360Ball::OnHit);
}

// Called every frame
void ABrickBreaker360Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector currVelocity = SphereCollider->GetPhysicsLinearVelocity();
	if (!IsSticky && !IsAttached && currVelocity.Size() != Velocity)
	{
		if (currVelocity.Normalize())
		{
			SphereCollider->SetPhysicsLinearVelocity(currVelocity * Velocity);
		}
	}
}


void ABrickBreaker360Ball::StartBall(FVector shootDir)
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

void ABrickBreaker360Ball::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (IsSticky && IsAttached) return;
	
	//UE_LOG(LogTemp, Warning, TEXT("Entered"))
}

void ABrickBreaker360Ball::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (IsSticky && IsAttached) return;

	//UE_LOG(LogTemp, Warning, TEXT("Left"))
}

void ABrickBreaker360Ball::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsSticky && IsAttached) return;

	if (OtherActor->GetFName().GetPlainNameString().Contains("base"))
	{
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
			//UE_LOG(LogTemp, Warning, TEXT("Magnitude:%f"), hitPointBasePos.Size())
			SphereCollider->SetSimulatePhysics(false);
			SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		if (APowerUpBase* powerUp = Cast<ABrickBreaker360Block>(OtherActor)->PowerUp)
		{
			FVector PowerUpVelocity = Hit.ImpactPoint - GetActorLocation();// *SphereCollider->GetPhysicsLinearVelocity();
			PowerUpVelocity.Normalize();
			PowerUpVelocity *= SphereCollider->GetPhysicsLinearVelocity()/2;
			powerUp->StartPowerUp(PowerUpVelocity);
		}
		OtherActor->Destroy();
	}
	//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *GetDebugName(OtherActor))
}