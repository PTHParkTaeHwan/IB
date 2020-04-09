// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_E_GS_Idle.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTTask_E_GS_Idle : public UBTTask_Wait
{
	GENERATED_BODY()
	
public:
	UBTTask_E_GS_Idle();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	float MaxIdleTime;
	float CurrentTime;
};
