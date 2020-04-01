// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "AIController.h"
#include "IB_E_GREATERSPIDER_AIController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API AIB_E_GREATERSPIDER_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AIB_E_GREATERSPIDER_AIController();
	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;

private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
	
	
	
};
