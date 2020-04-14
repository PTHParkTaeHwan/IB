// Fill out your copyright notice in the Description page of Project Settings.

#include "IBPlayerController.h"
#include "IBHUDWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "IBPlayerState.h"
#include "IBCharacter.h"
#include "IBGameplayWidget.h"
#include "Enemy/IB_E_GreaterSpider.h"

AIBPlayerController::AIBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIBHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UIBGameplayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}
}

void AIBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AIBPlayerController::OnGamePause);
	ABLOG(Warning, TEXT("SetupInputComponent"));
}

void AIBPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UIBGameplayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

void AIBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UIBHUDWidget>(this, HUDWidgetClass);
	ABCHECK(nullptr != HUDWidget);
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

void AIBPlayerController::NPCKill(AActor* KilledNPC /*AIBCharacter * KilledNPC*/) const
{
	auto Enemy = Cast<AIB_E_GreaterSpider>(KilledNPC);
	ABLOG(Warning, TEXT("%d"), Enemy->GetExp());
	IBPlayerState->AddExp(Enemy->GetExp());
}

void AIBPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;			
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}


