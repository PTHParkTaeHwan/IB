// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_E_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTDecorator_E_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_E_IsInAttackRange();


protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
