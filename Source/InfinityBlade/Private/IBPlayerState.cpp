// Fill out your copyright notice in the Description page of Project Settings.

#include "IBPlayerState.h"
#include "IBGameInstance.h"
#include "IBSaveGame.h"

AIBPlayerState::AIBPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	SaveSlotName = TEXT("Player1");
	CharacterIndex = 0;
}


void AIBPlayerState::InitPlayerData()
{
	auto IBSaveGame = Cast<UIBSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == IBSaveGame)
	{
		IBSaveGame = GetMutableDefault<UIBSaveGame>();
	}
	SetPlayerName(IBSaveGame->PlayerName);
	SetCharacterLevel(IBSaveGame->Level);
	GameScore = 0;
	GameHighScore = IBSaveGame->HighScore;
	Exp = IBSaveGame->Exp;
	CharacterIndex = IBSaveGame->CharacterIndex;
	SavePlayerData();
}

void AIBPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto IBGameInstance = Cast<UIBGameInstance>(GetGameInstance());
	ABCHECK(nullptr != IBGameInstance);

	CurrentStatData = IBGameInstance->GetIBCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;

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

	ABLOG(Warning, TEXT("AddExp : %d"), Exp);

	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
	return DidLevelUp;
}


int32 AIBPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}


void AIBPlayerState::SavePlayerData()
{
	UIBSaveGame* NewPlayerData = NewObject<UIBSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		ABLOG(Error, TEXT("Error!"));
	}
}

int32 AIBPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}
