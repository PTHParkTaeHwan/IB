// Fill out your copyright notice in the Description page of Project Settings.

#include "IB_E_GREATERSPIDER_AIController.h"

AIB_E_GREATERSPIDER_AIController::AIB_E_GREATERSPIDER_AIController()
{
	RepeatInterval = 2.0f;
}

void AIB_E_GREATERSPIDER_AIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AIB_E_GREATERSPIDER_AIController::OnRepeatTimer, RepeatInterval, true);
}

void AIB_E_GREATERSPIDER_AIController::UnPossess()
{
	Super::UnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AIB_E_GREATERSPIDER_AIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ABCHECK(nullptr != CurrentPawn);

	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UNavigationSystem::SimpleMoveToLocation(this, NextLocation.Location);
	}
}
