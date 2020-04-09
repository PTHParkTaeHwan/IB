// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_E_HitMotion.h"
#include "IB_E_GreaterSpider.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "IBCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTService_E_HitMotion::UBTService_E_HitMotion()
{
	NodeName = TEXT("E_HitMotion");
	Interval = 0.1f;
}

void UBTService_E_HitMotion::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AIB_E_GreaterSpider* GreaterSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetCharacter());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AIB_E_GREATERSPIDER_AIController::IsHitMotionKey, GreaterSpider->GetHitMotionOn());
}
