// Fill out your copyright notice in the Description page of Project Settings.

#include "IBHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "IBCharacterStatComponent.h"
#include "IBPlayerState.h"

void UIBHUDWidget::BindCharacterStat(class UIBCharacterStatComponent* CharacterStat)
{
	ABCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UIBHUDWidget::UpdateCharacterStat);
	CharacterStat->OnSEChanged.AddUObject(this, &UIBHUDWidget::UpdateCharacterStat);

}

void UIBHUDWidget::BindPlayerState(AIBPlayerState * PlayerState)
{
	ABCHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UIBHUDWidget::UpdatePlayerState);
}

void UIBHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	ABCHECK(nullptr != HPBar);

	SEBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbSE")));
	ABCHECK(nullptr != SEBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	ABCHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	ABCHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	ABCHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	ABCHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	ABCHECK(nullptr != HighScore);

}

void UIBHUDWidget::UpdateCharacterStat()
{
	ABCHECK(CurrentCharacterStat.IsValid());
	ABLOG(Warning, TEXT("SEBar %f"), CurrentCharacterStat->GetSERatio());
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	SEBar->SetPercent(CurrentCharacterStat->GetSERatio());
}

void UIBHUDWidget::UpdatePlayerState()
{
	ABCHECK(CurrentPlayerState.IsValid());
	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));

}
