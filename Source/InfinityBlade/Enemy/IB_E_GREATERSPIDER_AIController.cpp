// Fill out your copyright notice in the Description page of Project Settings.

#include "IB_E_GREATERSPIDER_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

//hit motion
const FName AIB_E_GREATERSPIDER_AIController::IsHitMotionKey(TEXT("IsHitMotion"));

//패트롤
const FName AIB_E_GREATERSPIDER_AIController::HomePosKey(TEXT("HomePos"));
const FName AIB_E_GREATERSPIDER_AIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AIB_E_GREATERSPIDER_AIController::TargetKey(TEXT("Target"));

//플레이어 발견
const FName AIB_E_GREATERSPIDER_AIController::IsRoarKey(TEXT("IsRoar"));

//접근 후 행동
const FName AIB_E_GREATERSPIDER_AIController::IsStayHereKey(TEXT("IsStayHere"));
const FName AIB_E_GREATERSPIDER_AIController::AttackOrIdleKey(TEXT("AttackOrIdle"));
const FName AIB_E_GREATERSPIDER_AIController::LeftOrRightKey(TEXT("LeftOrRight"));
const FName AIB_E_GREATERSPIDER_AIController::LeftPosKey(TEXT("LeftPos"));
const FName AIB_E_GREATERSPIDER_AIController::RightPosKey(TEXT("RightPos"));



AIB_E_GREATERSPIDER_AIController::AIB_E_GREATERSPIDER_AIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/dev/Enemy/AI/BB_GreaterSpider.BB_GreaterSpider"));
	if (BBObject.Succeeded())
	{
		BBAssset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/dev/Enemy/AI/BT_GreaterSpider.BT_GreaterSpider"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AIB_E_GREATERSPIDER_AIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	
	if (UseBlackboard(BBAssset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Warning, TEXT("not!"));
		}
	}
}


