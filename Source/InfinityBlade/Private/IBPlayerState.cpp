// Fill out your copyright notice in the Description page of Project Settings.

#include "IBPlayerState.h"
#include "IBGameInstance.h"

AIBPlayerState::AIBPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
}

int32 AIBPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AIBPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

float AIBPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	ABLOG(Warning, TEXT("ratio %f, Current %d, next %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool AIBPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;
	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	return DidLevelUp;
}

void AIBPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto IBGameInstance = Cast<UIBGameInstance>(GetGameInstance());
	ABCHECK(nullptr != IBGameInstance);

	CurrentStatData = IBGameInstance->GetIBCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;

}

void AIBPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	SetCharacterLevel(5);
	GameScore = 0;
	Exp = 0;
}
