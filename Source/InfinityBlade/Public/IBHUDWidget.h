// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Blueprint/UserWidget.h"
#include "IBHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIBHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UIBCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AIBPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();
	
private:
	TWeakObjectPtr<class UIBCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AIBPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* SEBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;
	
	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentScore;

	UPROPERTY()
	class UTextBlock* HighScore;
};
