// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Animation/AnimInstance.h"
#include "IB_E_GreaterSpider_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIB_E_GreaterSpider_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIB_E_GreaterSpider_AnimInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	
	
	
};
