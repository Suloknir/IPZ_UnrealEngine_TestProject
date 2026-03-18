// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkingTool.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values for this component's properties
UMarkingTool::UMarkingTool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UMarkingTool::SpawnSphere(FVector SpawnLocation, float Diameter)
{
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStaticMeshActor* NewSphere = World->SpawnActor<AStaticMeshActor>(SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	UStaticMeshComponent* MeshComp = NewSphere->GetStaticMeshComponent();
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(
		UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere")));
	UMaterialInterface* MaterialAsset = Cast<UMaterialInterface>(StaticLoadObject(
		UMaterialInterface::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")));
	MeshComp->SetStaticMesh(MeshAsset);
	MeshComp->SetMaterial(0, MaterialAsset);
	float ScaleFactor = Diameter / 100.0f;
	NewSphere->SetActorScale3D(FVector(ScaleFactor));
	ActiveSpheres.Add(NewSphere);
}

void UMarkingTool::DrawCursor(FVector Location)
{
	DrawDebugSphere(
		GetWorld(),
		Location,
		3.0f,
		12,
		FColor::Yellow,
		false,
		-1,
		0,
		0.75f);
}


// // Called when the game starts
// void UMarkingTool::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	// ...
// 	
// }


// // Called every frame
// void UMarkingTool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }
