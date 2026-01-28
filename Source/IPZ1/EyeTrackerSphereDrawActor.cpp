// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeTrackerSphereDrawActor.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEyeTrackerSphereDrawActor::AEyeTrackerSphereDrawActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere_cursor"));
	RootComponent = CursorMesh;
	CursorMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AEyeTrackerSphereDrawActor::BeginPlay()
{
	Super::BeginPlay();
	GazeCursorInstance = GetWorld()->SpawnActor<AActor>(GazeCursorClass);
}

// Called every frame
void AEyeTrackerSphereDrawActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FEyeTrackerGazeData GazeData;
	if (UEyeTrackerFunctionLibrary::GetGazeData(GazeData))
	{
		UE_LOG(LogTemp, Warning, TEXT("EyeTracker Direction: %s"), *GazeData.GazeDirection.ToString());
		FVector CollisionPoint;
		if (GetGazeCollisionPoint(GazeData, CollisionPoint))
			DrawDebugSphere(
				GetWorld(),
				CollisionPoint,
				8.5f,
				12,
				FColor::Cyan,
				false,
				-1,
				0,
				2.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EyeTracker not detected"));
		DrawDebugSphere(
			GetWorld(),
			FVector(0, 0, 0),
			20.0f,
			12,
			FColor::Red,
			false,
			-1,
			0,
			2.0f);
	}
	
}

bool AEyeTrackerSphereDrawActor::GetGazeCollisionPoint(const FEyeTrackerGazeData& GazeData, FVector& outCollisionPoint)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController || !PlayerController->PlayerCameraManager)
	{
		return false;
	}
	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector Start = CameraLocation;
	FVector WorldDirection = CameraRotation.RotateVector(GazeData.GazeDirection);
	WorldDirection.Normalize();

	float TraceLength = 10000.0f; // 100 metrów
	FVector End = Start + (WorldDirection * TraceLength);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerController);
	QueryParams.AddIgnoredActor(this); 

	bool isHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams);

	if (isHit)
	{
		outCollisionPoint = HitResult.Location;
		return true;
	}
	return false;
}

