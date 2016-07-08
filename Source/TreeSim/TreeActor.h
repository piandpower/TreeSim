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
	float stiffness;
	float branchSuppressPower;
	float branchSwayPowerA, branchSwayPowerB;
	branch(FName inName, float inBranchSuppressPower = .6f, float inBranchSwayPowerA = 0.3f, float inBranchSwayPowerB = 0.3f) :
		name(inName), 
		branchMovementRandomisation(FMath::Rand()),
		branchSuppressPower(inBranchSuppressPower),
		branchSwayPowerA(inBranchSwayPowerA),
		branchSwayPowerB(inBranchSwayPowerB) {}
	branch(){}
};

UCLASS()
class TREESIM_API ATreeActor : public AActor
{
	GENERATED_BODY()

	float RunningTime=0.f;
	float delayedWindPower = 0.f, delayedTime = 0.f;
	float windOffset = FMath::Rand()*0.5f;
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
		float trunkMass = 1.f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float trunkDampingCoefficient = 1.f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float trunkStiffnessCoefficient = 1.f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float branchMass = 1.f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float branchDampingCoefficient = 1.f;

	UPROPERTY(EditAnywhere, Category = Tree)
		float branchStiffnessCoefficient = 1.f;

	UPROPERTY(EditAnywhere, Category = TreeWind)
		AWindDirectionalSource *windSource;

	TArray<branch> branches;
	branch trunkSegment;
};
