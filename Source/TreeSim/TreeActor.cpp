// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeSim.h"
#include "TreeActor.h"

// Sets default values
ATreeActor::ATreeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TreeMeshComponent = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("TreeMesh"));

	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("SkeletalMesh'/Game/Tree/armaturedFixed.armaturedFixed'"));
	TreeMeshComponent->SetSkeletalMesh(MeshObj.Object);

	//Input: Name, branch noise, branch suppress power, swaypowerA, swayPowerB
	branches.Add(branch(FName("bone001_000")));
	branches.Add(branch(FName("bone002_000")));
	branches.Add(branch(FName("bone003_000")));
	branches.Add(branch(FName("bone004_000")));
	branches.Add(branch(FName("bone005_000")));
	branches.Add(branch(FName("bone006_000")));
	branches.Add(branch(FName("bone007_000")));
	branches.Add(branch(FName("bone008_000")));
	branches.Add(branch(FName("bone009_000")));
	branches.Add(branch(FName("bone010_000")));
	branches.Add(branch(FName("bone011_000")));
	branches.Add(branch(FName("bone012_000")));
	branches.Add(branch(FName("bone013_000")));
	branches.Add(branch(FName("bone014_000")));
	branches.Add(branch(FName("bone015_000")));
	branches.Add(branch(FName("bone016_000")));
	branches.Add(branch(FName("bone017_000")));
	branches.Add(branch(FName("bone018_000")));
	branches.Add(branch(FName("bone019_000")));
	branches.Add(branch(FName("bone020_000")));
	branches.Add(branch(FName("bone021_000")));
	branches.Add(branch(FName("bone022_000")));
	branches.Add(branch(FName("bone023_000")));
	branches.Add(branch(FName("bone024_000")));
	branches.Add(branch(FName("bone025_000")));
	branches.Add(branch(FName("bone026_000")));
	branches.Add(branch(FName("bone027_000")));
	branches.Add(branch(FName("bone028_000")));
	branches.Add(branch(FName("bone029_000")));
	branches.Add(branch(FName("bone030_000")));
	branches.Add(branch(FName("bone031_000")));
	branches.Add(branch(FName("bone032_000")));
	branches.Add(branch(FName("bone033_000")));
	branches.Add(branch(FName("bone034_000")));
	branches.Add(branch(FName("bone035_000")));
	branches.Add(branch(FName("bone036_000")));
	branches.Add(branch(FName("bone037_000")));
	branches.Add(branch(FName("bone038_000")));
	branches.Add(branch(FName("bone039_000")));

	trunkSegment = branch(FName("trunk1"));
}

// Called when the game starts or when spawned
void ATreeActor::BeginPlay()
{
	Super::BeginPlay();
	FRotator branchRotator;
	for (int i = 0; i < branches.Num(); i++){
		branches[i].restingRot = TreeMeshComponent->GetBoneQuaternion(branches[i].name, EBoneSpaces::WorldSpace);
		branchRotator = TreeMeshComponent->GetBoneRotationByName(branches[i].name, EBoneSpaces::WorldSpace);
		branches[i].restingRotationVector = branchRotator.Euler();
		branches[i].restingRotationVector.Normalize();
	}

	trunkSegment.restingRot = TreeMeshComponent->GetBoneQuaternion(trunkSegment.name, EBoneSpaces::WorldSpace);
}


float lerp(float a, float b, float c){
	return a + c * (b - a);
}

FQuat quatAroundY(float angle){
	float sinha = sin(angle*0.5);
	float cosha = cos(angle*0.5);
	return FQuat(0.f, sinha, 0.f, cosha);
}

static int runCount = 0;

float calculateWindPower(float time) {
	return cos(time) * cos(time * 3.f) * cos(time * 5.f) * cos(time * 7.f) + sin(time * 25.f) * 1.f;
}

// Called every frame
void ATreeActor::Tick( float DeltaTime )
{
	//Update running time
	RunningTime += DeltaTime;
	Super::Tick(DeltaTime);


	//Get wind direction & calculate wind tangent
	FVector windDirection = windSource->GetActorRotation().Euler();

	windDirection.Normalize();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, windDirection.ToString());
	FVector windTangent = FVector(-windDirection.Z, windDirection.Y, windDirection.X);

	//Bend the trunk
	float timeMult = RunningTime / 3.f;
	float trunkMovementMult = 4.f;
	float windPower = calculateWindPower(timeMult);
	FVector trunkMovement = FVector((windTangent.X)*windPower*trunkMovementMult, (windTangent.Y)*windPower*trunkMovementMult, (windTangent.Z)*windPower*trunkMovementMult);
	//FQuat trunkRotation = FQuat(FVector::UpVector.Rotation());
	FQuat trunkRotation = trunkSegment.restingRot;
	trunkRotation *= FQuat::MakeFromEuler(trunkMovement);
	TreeMeshComponent->SetBoneRotationByName(trunkSegment.name, FRotator(trunkRotation), EBoneSpaces::WorldSpace);


	FQuat branchQuat;
	float a, oldA, b;
	float branchNoise = 1.f;
	for (int i = 0; i < branches.Num(); i++){
		branches[i].restingRotationVector = TreeMeshComponent->GetBoneRotationByName(branches[i].name, EBoneSpaces::WorldSpace).Euler();

		//How much is the branch facing the wind
		float facingWind = FVector::DotProduct(FVector(branches[i].restingRotationVector.X, 0.f, branches[i].restingRotationVector.Z), windDirection)/180.f;

		oldA = branches[i].branchSwayPowerA * cos(timeMult + branchNoise * branches[i].branchMovementRandomisation);

		a = -0.5f * oldA + branches[i].branchSuppressPower * branches[i].branchSwayPowerA;

		b = branches[i].branchSwayPowerB * cos(delayedTime + branchNoise * branches[i].branchMovementRandomisation) * windPower;

		a = lerp(oldA * windPower, a * windPower, delayedWindPower * std::fmin(std::fmax(1.f-facingWind, 0.f), 1.f));

		FQuat rotation1 = FQuat(windTangent, a);
		FQuat rotation2 = FQuat(FVector::UpVector, b);
		rotation1.Normalize();
		rotation2.Normalize();

		FQuat endRotation = FQuat::FastLerp(rotation1, rotation2, 1 - abs(facingWind));
		
		endRotation = branches[i].restingRot * endRotation;
		endRotation.Normalize();
		
		TreeMeshComponent->SetBoneRotationByName(branches[i].name, FRotator(endRotation), EBoneSpaces::WorldSpace);

	}
	runCount++;
	delayedTime = timeMult;
	delayedWindPower = (windPower + delayedWindPower)/2;
}