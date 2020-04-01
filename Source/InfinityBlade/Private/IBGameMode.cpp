// Fill out your copyright notice in the Description page of Project Settings.

#include "IBGameMode.h"
#include "IBPawn.h"
#include "IBCharacter.h"
#include "IBPlayerController.h"
#include "IBPlayerState.h"
#include "IBGameState.h"


AIBGameMode::AIBGameMode()
{
	DefaultPawnClass = AIBCharacter::StaticClass();
	PlayerControllerClass = AIBPlayerController::StaticClass();
	PlayerStateClass = AIBPlayerState::StaticClass();
	GameStateClass = AIBGameState::StaticClass();
}

void AIBGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto IBPlayerState = Cast<AIBPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != IBPlayerState);
	IBPlayerState->InitPlayerData();
}
