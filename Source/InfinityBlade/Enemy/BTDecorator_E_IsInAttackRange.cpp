// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_E_IsInAttackRange.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "IB_E_GreaterSpider.h"
#include "IBCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTDecorator_E_IsInAttackRange::UBTDecorator_E_IsInAttackRange()
{
	NodeName = TEXT("E_GreaterSpiderCanAttack");
}

bool UBTDecorator_E_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AIBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIB_E_GREATERSPIDER_AIController::TargetKey));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 350.0f);
	auto IBGreaterSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == IBGreaterSpider)
		return false;
	if (bResult)
	{
		if (!IBGreaterSpider->GetIsAttacking())
		{
			IBGreaterSpider->SetCharacterInAttackRange(true);
			IBGreaterSpider->SetTentionMode();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AIB_E_GREATERSPIDER_AIController::IsStayHereKey, IBGreaterSpider->GetIsStayHere());
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AIB_E_GREATERSPIDER_AIController::AttackOrIdleKey, IBGreaterSpider->GetAttackOrIdle());
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AIB_E_GREATERSPIDER_AIController::LeftOrRightKey, IBGreaterSpider->GetLeftOrRight());
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AIB_E_GREATERSPIDER_AIController::LeftPosKey,
				(IBGreaterSpider->GetActorLocation() - IBGreaterSpider->GetActorRightVector() * 230.0f)
			);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AIB_E_GREATERSPIDER_AIController::RightPosKey,
				(IBGreaterSpider->GetActorLocation() + IBGreaterSpider->GetActorRightVector() * 230.0f)
			);
			
			DrawDebugPoint(GetWorld(), IBGreaterSpider->GetActorLocation() - IBGreaterSpider->GetActorRightVector() * 230.0f, 20.0f, FColor::Red, false, 3.0f);
			DrawDebugPoint(GetWorld(), IBGreaterSpider->GetActorLocation() + IBGreaterSpider->GetActorRightVector() * 230.0f, 20.0f, FColor::Blue, false, 3.0f);

		}
		return bResult;
	}
	else
	{
		IBGreaterSpider->SetCharacterInAttackRange(false);
		if (IBGreaterSpider->GetIsRoar())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AIB_E_GREATERSPIDER_AIController::IsRoarKey, true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AIB_E_GREATERSPIDER_AIController::IsRoarKey, false);
		}
		return bResult;
	}
}
