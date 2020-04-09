// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_E_HitMotion.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UBTService_E_HitMotion : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_E_HitMotion();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float DetectRadius;
	
	
};
