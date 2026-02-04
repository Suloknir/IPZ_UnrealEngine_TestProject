// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeTrackerSphereDrawerComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UEyeTrackerSphereDrawerComponent::UEyeTrackerSphereDrawerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEyeTrackerSphereDrawerComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Camera = Owner->FindComponentByClass<UCameraComponent>();
		if (!Camera)
			UE_LOG(LogTemp, Error, TEXT("EyeTrackerComponent: Camera not found in %s!"), *Owner->GetName());
	}
	
}


// Called every frame
void UEyeTrackerSphereDrawerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// UE_LOG(LogTemp, Warning, TEXT("Camera location: %s", Camera->GetComponentLocation()));
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FEyeTrackerGazeData GazeData;
	if (UEyeTrackerFunctionLibrary::GetGazeData(GazeData))
	{
		// Opcjonalnie: Logowanie (możesz wyłączyć, żeby nie spamić konsoli)
		// UE_LOG(LogTemp, Warning, TEXT("EyeTracker Direction: %s"), *GazeData.GazeDirection.ToString());

		FVector CollisionPoint;
		if (GetGazeCollisionPoint(GazeData, CollisionPoint))
		{
			DrawDebugSphere(
				GetWorld(),
				CollisionPoint,
				8.5f,
				12,
				FColor::Green,
				false,
				-1,
				0,
				2.0f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EyeTracker not detected"));
		DrawDebugSphere(
			GetWorld(),
			Camera->GetComponentLocation(),
			20.0f, 
			12,
			FColor::Red,
			false,
			-1,
			0,
			2.0f);
}
}

bool UEyeTrackerSphereDrawerComponent::GetGazeCollisionPoint(const FEyeTrackerGazeData& GazeData, FVector& outCollisionPoint)
{
	if (!Camera) return false;
	FVector Start = Camera->GetComponentLocation();
	// FVector WorldDirection = GazeData.GazeDirection; 
	// WorldDirection.Normalize();

	float TraceLength = 10000.0f; //100 meters
	FVector End = Start + (GazeData.GazeDirection * TraceLength);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	//Ignore for pawn
	if (GetOwner())
		QueryParams.AddIgnoredActor(GetOwner());

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

