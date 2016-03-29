// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeSim.h"
#include "TreeAnimInstance.h"





//This function is like PostInitAnimtree in UE3
void UTreeAnimInstance::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();
}

//Tick
void UTreeAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!OwningPawn)
	{
		return;
	}

	//Set whether moving or not
	//IsMoving = (OwningPawn->GetVelocity().SizeSquared() > 25);
	OwningPawn->AddActorLocalRotation(FRotator(0, 50.0f, 50.0f));

	
}