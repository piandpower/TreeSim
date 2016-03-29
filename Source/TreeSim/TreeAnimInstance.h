// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "TreeAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UTreeAnimInstance : public UAnimInstance{
	GENERATED_BODY()
public:
	/** Is Moving */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool IsMoving;

	//init and tick
public:

	APawn * OwningPawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};