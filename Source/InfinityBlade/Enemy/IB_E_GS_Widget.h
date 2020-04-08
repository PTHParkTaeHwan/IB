// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Blueprint/UserWidget.h"
#include "IB_E_GS_Widget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIB_E_GS_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	void BindCharacterStat(class UIB_E_GS_StatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UIB_E_GS_StatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;

	
	
	
};
