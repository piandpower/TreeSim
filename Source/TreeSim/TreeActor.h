// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <CoreMisc.h>
#include "GameFramework/Actor.h"
#include "Components/PoseableMeshComponent.h"
#include "TreeActor.generated.h"

struct branch{
	FName name;
	FQuat restingRot;
	FVector restingRotationVector;
	float branchMovementRandomisation;
	branch(FName inName) :
		name(inName), 
		branchMovementRandomisation(FMath::Rand()) {}
	branch(){}
};

UCLASS()
class TREESIM_API ATreeActor : public AActor
{
	GENERATED_BODY()

	float RunningTime=0.f;
	float delayedWindPower = 0.f, delayedTime = 0.f;
	float windOffset = FMath::Rand()*0.5f;
	float calculateWindPower(float time);
public:	
	// Sets default values for this actor's properties
	ATreeActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool first = true;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	TArray<branch> ReturnBranchData();

	UPROPERTY(EditAnywhere, Category = Tree)
		UPoseableMeshComponent *TreeMeshComponent;

	UPROPERTY(EditAnywhere, Category = Tree)
		float windIntensity = 1.f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float branchSuppressPower = .6f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float branchSwayPowerA = 0.3f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float branchSwayPowerB = 0.3f;

	UPROPERTY(EditAnywhere, Category = TreeWind)
		AWindDirectionalSource *windSource;

	TArray<branch> branches;
	branch trunkSegment;
};
