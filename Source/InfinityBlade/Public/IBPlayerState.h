// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/PlayerState.h"
#include "IBPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

UCLASS()
class INFINITYBLADE_API AIBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AIBPlayerState();
	
	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	
//경험치 표시 멤버함수, 변수
public:
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);

protected:
	UPROPERTY(Transient)
	int32 Exp;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FIBCharacterData* CurrentStatData;
	

//end Exp
public:
	void InitPlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;
	
};
