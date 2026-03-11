// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MarkingTool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IPZ1_API UMarkingTool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMarkingTool();
	
	UFUNCTION(BlueprintCallable, Category = "Marking")
	void SpawnSphere(FVector SpawnLocation, float Diameter = 50.0f);
	
	// UFUNCTION(BlueprintCallable, Category = "Marking")
	// void RemoveSphere(AActor* TargetActor);

// protected:
	// Called when the game starts
	// virtual void BeginPlay() override;
	
private:
	TArray<AActor*> ActiveSpheres;
	
// public:	
// 	// Called every frame
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
