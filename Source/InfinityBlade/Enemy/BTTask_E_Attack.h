// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_E_Attack.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTTask_E_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_E_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;

	UBehaviorTreeComponent* OwnerComponent;

public:
	void SetLeftPos(FVector NewVector);
	
};