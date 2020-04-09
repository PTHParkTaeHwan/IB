// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_E_GS_Wait.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "IB_E_GreaterSpider.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_E_GS_Wait::UBTTask_E_GS_Wait()
{
	NodeName = TEXT("E_GS_HitMotion");
	bNotifyTick = true;
	
}

EBTNodeResult::Type UBTTask_E_GS_Wait::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto IBGreaterSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == IBGreaterSpider)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_E_GS_Wait::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	auto IBGreaterSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == IBGreaterSpider) return;

	if (!IBGreaterSpider->GetHitMotionOn())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
