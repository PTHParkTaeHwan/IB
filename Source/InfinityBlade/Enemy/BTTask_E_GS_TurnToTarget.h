// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_E_GS_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTTask_E_GS_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_E_GS_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
	
};
