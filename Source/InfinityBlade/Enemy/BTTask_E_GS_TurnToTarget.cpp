// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_E_GS_TurnToTarget.h"
#include "IB_E_GreaterSpider.h"
#include "IBCharacter.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_E_GS_TurnToTarget::UBTTask_E_GS_TurnToTarget()
{
	NodeName = TEXT("E_GS_Turn");

}

EBTNodeResult::Type UBTTask_E_GS_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto GreatSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == GreatSpider)
		return EBTNodeResult::Failed;

	auto Target = Cast<AIBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIB_E_GREATERSPIDER_AIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - GreatSpider->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	GreatSpider->SetActorRotation(FMath::RInterpTo(GreatSpider->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
