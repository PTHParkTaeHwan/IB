// Fill out your copyright notice in the Description page of Project Settings.

#include "IBGameplayWidget.h"
#include "Components/Button.h"
#include "IBPlayerController.h"

void UIBGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UIBGameplayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UIBGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UIBGameplayWidget::OnRetryGameClicked);
	}
}


void UIBGameplayWidget::OnResumeClicked()
{
	auto IBPlayerController = Cast<AIBPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != IBPlayerController);

	RemoveFromParent();

	IBPlayerController->ChangeInputMode(true);
	IBPlayerController->SetPause(false);
}

void UIBGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UIBGameplayWidget::OnRetryGameClicked()
{
}
