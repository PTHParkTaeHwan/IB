// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "BehaviorTree/BTService.h"
#include "BTService_E_IsRoar.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTService_E_IsRoar : public UBTService
{
	GENERATED_BODY()
	
	UBTService_E_IsRoar();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float DetectRadius;

	
};
