// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_E_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTTask_E_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_E_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
