// Fill out your copyright notice in the Description page of Project Settings.

#include "IB_E_GS_Widget.h"
#include "IB_E_GS_StatComponent.h"
#include "Components/ProgressBar.h"

void UIB_E_GS_Widget::BindCharacterStat(class UIB_E_GS_StatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	CurrentCharacterStat->E_OnHPChanged.AddUObject(this, &UIB_E_GS_Widget::UpdateHPWidget);
}

void UIB_E_GS_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UIB_E_GS_Widget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
