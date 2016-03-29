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


	branches.Add(branch(FName("bone001_000"), FMath::SRand()));
	branches.Add(branch(FName("bone002_000"), FMath::SRand()));
	branches.Add(branch(FName("bone003_000"), FMath::SRand()));
	branches.Add(branch(FName("bone004_000"), FMath::SRand()));
	branches.Add(branch(FName("bone005_000"), FMath::SRand()));
	branches.Add(branch(FName("bone006_000"), FMath::SRand()));
	branches.Add(branch(FName("bone007_000"), FMath::SRand()));
	branches.Add(branch(FName("bone008_000"), FMath::SRand()));
	branches.Add(branch(FName("bone009_000"), FMath::SRand()));
	branches.Add(branch(FName("bone010_000"), FMath::SRand()));
	branches.Add(branch(FName("bone011_000"), FMath::SRand()));
	branches.Add(branch(FName("bone012_000"), FMath::SRand()));
	branches.Add(branch(FName("bone013_000"), FMath::SRand()));
	branches.Add(branch(FName("bone014_000"), FMath::SRand()));
	branches.Add(branch(FName("bone015_000"), FMath::SRand()));
	branches.Add(branch(FName("bone016_000"), FMath::SRand()));
	branches.Add(branch(FName("bone017_000"), FMath::SRand()));
	branches.Add(branch(FName("bone018_000"), FMath::SRand()));
	branches.Add(branch(FName("bone019_000"), FMath::SRand()));
	branches.Add(branch(FName("bone020_000"), FMath::SRand()));
	branches.Add(branch(FName("bone021_000"), FMath::SRand()));
	branches.Add(branch(FName("bone022_000"), FMath::SRand()));
	branches.Add(branch(FName("bone023_000"), FMath::SRand()));
	branches.Add(branch(FName("bone024_000"), FMath::SRand()));
	branches.Add(branch(FName("bone025_000"), FMath::SRand()));
	branches.Add(branch(FName("bone026_000"), FMath::SRand()));
	branches.Add(branch(FName("bone027_000"), FMath::SRand()));
	branches.Add(branch(FName("bone028_000"), FMath::SRand()));
	branches.Add(branch(FName("bone029_000"), FMath::SRand()));
	branches.Add(branch(FName("bone030_000"), FMath::SRand()));
	branches.Add(branch(FName("bone031_000"), FMath::SRand()));
	branches.Add(branch(FName("bone032_000"), FMath::SRand()));
	branches.Add(branch(FName("bone033_000"), FMath::SRand()));
	branches.Add(branch(FName("bone034_000"), FMath::SRand()));
	branches.Add(branch(FName("bone035_000"), FMath::SRand()));
	branches.Add(branch(FName("bone036_000"), FMath::SRand()));
	branches.Add(branch(FName("bone037_000"), FMath::SRand()));
	branches.Add(branch(FName("bone038_000"), FMath::SRand()));
	branches.Add(branch(FName("bone039_000"), FMath::SRand()));


	//Trunk segments
	trunkSegments.Add(branch(FName("trunk1"), FMath::SRand()));
	trunkSegments.Add(branch(FName("trunk2"), FMath::SRand()));
	trunkSegments.Add(branch(FName("trunk3"), FMath::SRand()));
	trunkSegments.Add(branch(FName("trunk4"), FMath::SRand()));
}

// Called when the game starts or when spawned
void ATreeActor::BeginPlay()
{
	Super::BeginPlay();
	FRotator branchRotator;
	for (int i = 0; i < branches.Num(); i++){
		branches[i].restingRot = TreeMeshComponent->GetBoneQuaternion(branches[i].name, EBoneSpaces::WorldSpace);
		//branchRotator = TreeMeshComponent->GetBoneRotationByName(branches[i].name, EBoneSpaces::WorldSpace);
		//branches[i].restingRotationVector = FVector(branchRotator.Pitch, branchRotator.Roll, branchRotator.Yaw);
		
		//branches[i].restingRotationVector = TreeMeshComponent->GetBoneRotationByName(branches[i].name, EBoneSpaces::ComponentSpace).Vector();
		branchRotator = TreeMeshComponent->GetBoneRotationByName(branches[i].name, EBoneSpaces::WorldSpace);
		branches[i].restingRotationVector = branchRotator.Euler();
		branches[i].restingRotationVector.Normalize();
	}

	for (int i = 0; i < trunkSegments.Num(); i++){
		trunkSegments[i].restingRot = TreeMeshComponent->GetBoneQuaternion(trunkSegments[i].name, EBoneSpaces::WorldSpace);
	}
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
// Called every frame
void ATreeActor::Tick( float DeltaTime )
{
	//Update running time
	RunningTime += DeltaTime;
	Super::Tick(DeltaTime);


	//Get wind direction & calculate wind tangent
	FVector windRotationVector = windSource->GetActorRotation().Euler();
	windRotationVector.Normalize();
	FVector windTangent = FVector(-windRotationVector.Z, windRotationVector.Y, windRotationVector.X);

	//Calculate wind power
	float timeMult = RunningTime / 10.f;
	float windPower = cos(timeMult * PI) * cos(timeMult * 3.f * PI) * cos(timeMult * 5.f * PI) * cos(timeMult * 7.f * PI) + sin(timeMult * 25.f * PI) * 1.f;

	//Bend the trunk
	float trunkMovementMult = 2.f;
	FVector trunkMovement = FVector((windTangent.X)*windPower*trunkMovementMult, (windTangent.Y)*windPower*trunkMovementMult, (windTangent.Z)*windPower*trunkMovementMult);
	//FQuat trunkRotation = FQuat(FVector::UpVector.Rotation());
	FQuat trunkRotation;
	for (int i = 0; i < trunkSegments.Num(); i++){
		trunkRotation = trunkSegments[i].restingRot;
		trunkRotation *= FQuat::MakeFromEuler(trunkMovement);
		TreeMeshComponent->SetBoneRotationByName(trunkSegments[i].name, FRotator(trunkRotation), EBoneSpaces::WorldSpace);
	}
	//rot.Pitch = windPower;
	//rot.Pitch += 0.05 * cos(RunningTime);
	//TreeMeshComponent->SetBoneRotationByName(FName("trunk1"), FRotator(trunkRotation), EBoneSpaces::WorldSpace);


	FQuat branchQuat;
	float a, oldA, b;
	float branchSuppressPower = 0.7f;
	float branchSwayPowerA = 0.2f, branchSwayPowerB = 0.3f, branchNoise = 0.9f;
	for (int i = 0; i < branches.Num(); i++){
		//How much is the branch facing the wind
		
		float facingWind = FVector::DotProduct(FVector(branches[i].restingRotationVector.X, 0.f, branches[i].restingRotationVector.Z), windRotationVector);

		oldA = branchSwayPowerA * cos(timeMult + branchNoise * branches[i].branchMovementRandomisation);

		a = -0.5f * oldA + branchSuppressPower * branchSwayPowerA;

		b = branchSwayPowerB * cos(timeMult + branchNoise * branches[i].branchMovementRandomisation) * windPower;

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
	delayedWindPower = windPower;
}