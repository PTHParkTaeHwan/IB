// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/GameStateBase.h"
#include "IBGameState.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API AIBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AIBGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();

private:
	UPROPERTY(Transient)
	int32 TotalGameScore;
	
	
	
};
