// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_E_GS_MoveTo.h"
#include "IB_E_GreaterSpider.h"
#include "IBCharacter.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_E_GS_MoveTo::UBTTask_E_GS_MoveTo()
{
	NodeName = TEXT("SetLeftPos");
}

EBTNodeResult::Type UBTTask_E_GS_MoveTo::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto GreatSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<AIBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIB_E_GREATERSPIDER_AIController::TargetKey));

	if (!GreatSpider->GetIsLeftOrRightMove())
	{
		GreatSpider->PlayLeftOrRightMontage();
		if (!GreatSpider->GetIsLeftOrRightMove())
		{
			GreatSpider->TentionModeInit();
			return EBTNodeResult::Failed;
		}
	}

	GreatSpider->SetbOrientRotationToMovement(false);
	GreatSpider->GetCharacterMovement()->MaxWalkSpeed = 200.0f;


	return EBTNodeResult::InProgress;
}

void UBTTask_E_GS_MoveTo::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	auto GreatSpider = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<AIBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIB_E_GREATERSPIDER_AIController::TargetKey));
	
	FVector LookVector = Target->GetActorLocation() - GreatSpider->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	GreatSpider->SetActorRotation(FMath::RInterpTo(GreatSpider->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 4.0f));

}
