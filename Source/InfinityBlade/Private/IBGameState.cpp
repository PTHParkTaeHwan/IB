// Fill out your copyright notice in the Description page of Project Settings.

#include "IBGameState.h"

AIBGameState::AIBGameState()
{
	TotalGameScore = 0;
}

int32 AIBGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AIBGameState::AddGameScore()
{
	TotalGameScore++;
}
