// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_E_GS_Idle.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "IB_E_GreaterSpider.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_E_GS_Idle::UBTTask_E_GS_Idle()
{
	NodeName = TEXT("E_GS_HitMotion");

	MaxIdleTime = 2.0f;
	CurrentTime = 0.0f;	
}

EBTNodeResult::Type UBTTask_E_GS_Idle::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto IBGreaterSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == IBGreaterSpider)
		return EBTNodeResult::Failed;
	
	CurrentTime += 0.5f;
	ABLOG(Warning, TEXT("UBTTask_E_GS_Idle CurrentTime %f"), CurrentTime);

	if (CurrentTime >= MaxIdleTime)
	{
		CurrentTime = 0.0f;
		IBGreaterSpider->TentionModeInit();
		IBGreaterSpider->SetIsAttacking(false);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::InProgress;
}



