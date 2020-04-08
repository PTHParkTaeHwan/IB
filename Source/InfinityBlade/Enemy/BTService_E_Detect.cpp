// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_E_Detect.h"
#include "IB_E_GreaterSpider.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "IBCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTService_E_Detect::UBTService_E_Detect()
{
	NodeName = TEXT("E_Detect_GreaterSpider");
	Interval = 0.5f;
	DetectRadius = 500.0f;
}

void UBTService_E_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AIB_E_GreaterSpider* Enemy = Cast<AIB_E_GreaterSpider>(OwnerComp.GetAIOwner()->GetCharacter());
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	if (bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AIB_E_GREATERSPIDER_AIController::TargetKey, nullptr);
		for (auto OverlapResult : OverlapResults)
		{
			AIBCharacter* IBCharacter = Cast<AIBCharacter>(OverlapResult.GetActor());
			if (IBCharacter && IBCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AIB_E_GREATERSPIDER_AIController::TargetKey, IBCharacter);
				DetectRadius = 4000.0f;
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, IBCharacter->GetActorLocation(), 20.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), IBCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				//ControllingCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;
				Enemy->SetEnemyMode(EnemyMode::TARGETON);
				Enemy->SetHPBarWidgetHiddenInGame(false);
				return;
			}
		}
	}
	else
	{
		//ControllingCharacter->GetCharacterMovement()->MaxWalkSpeed = 60;
		DetectRadius = 2000.0f;
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AIB_E_GREATERSPIDER_AIController::TargetKey, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}


}
