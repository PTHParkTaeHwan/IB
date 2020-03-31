// Fill out your copyright notice in the Description page of Project Settings.

#include "IBPlayerController.h"
#include "IBHUDWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "IBPlayerState.h"
#include "IBCharacter.h"

AIBPlayerController::AIBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIBHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

void AIBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UIBHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	IBPlayerState = Cast<AIBPlayerState>(PlayerState);
	ABCHECK(nullptr != IBPlayerState);
	HUDWidget->BindPlayerState(IBPlayerState);
	IBPlayerState->OnPlayerStateChanged.Broadcast();
}

UIBHUDWidget * AIBPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AIBPlayerController::NPCKill(AIBCharacter * KilledNPC) const
{
	IBPlayerState->AddExp(KilledNPC->GetExp());
}


