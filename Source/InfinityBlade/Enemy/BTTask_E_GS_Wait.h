// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_E_GS_Wait.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTTask_E_GS_Wait : public UBTTask_Wait
{
	GENERATED_BODY()
	
public:	
	UBTTask_E_GS_Wait();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
