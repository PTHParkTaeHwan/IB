// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/SaveGame.h"
#include "IBSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIBSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UIBSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;
	
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;
	
	UPROPERTY()
	int32 CharacterIndex;
};
