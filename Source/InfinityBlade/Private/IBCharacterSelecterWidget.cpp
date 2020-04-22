// Fill out your copyright notice in the Description page of Project Settings.

#include "IBCharacterSelecterWidget.h"
#include "IBCharacterSetting.h"
#include "IBGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "IBSaveGame.h"
#include "IBPlayerState.h"


void UIBCharacterSelecterWidget::NextCharacter(bool bForward)
{
	//11, 14
	bForward ? CurrentIndex++ : CurrentIndex--;
	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	CurrentIndex = 14;
	auto CharacterSetting = GetDefault<UIBCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];
	ABLOG(Warning, TEXT("CurrentIndex %d"), CurrentIndex);

	auto IBGameInstance = GetWorld()->GetGameInstance<UIBGameInstance>();
	ABCHECK(nullptr != IBGameInstance);
	ABCHECK(TargetComponent.IsValid());


	USkeletalMesh* Asset = IBGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
		TargetComponent2->SetSkeletalMesh(Asset);
	}

	CurrentIndex = 14;
	AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];
	Asset = IBGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
	}
	
	
}

void UIBCharacterSelecterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UIBCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		if (!TargetComponent.IsValid())
		{
			TargetComponent = It->GetSkeletalMeshComponent();
			continue;
		}
		if (!TargetComponent2.IsValid())
		{
			TargetComponent2 = It->GetSkeletalMeshComponent();
			break;
		}
	}
	
	
	//for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	//{
	//	
	//	
	//}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &UIBCharacterSelecterWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UIBCharacterSelecterWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UIBCharacterSelecterWidget::OnConfirmClicked);

	NextCharacter(true);

	bLeftFirstClicked = false;
	bRightFirstClicked = false;
	fLevelOpenBufferTime = 0.0f;
}

void UIBCharacterSelecterWidget::OnPrevClicked()
{
	//NextCharacter(false);
	if (!bLeftFirstClicked)
	{
		bLeftFirstClicked = true;
		bRightFirstClicked = true;
		CurrentIndex = 14;
		ABLOG(Warning, TEXT("CurrentIndex %d"), CurrentIndex);
		return ;
	}
	if (CurrentIndex == 11)
	{
		CurrentIndex = 14;
	}
	else if (CurrentIndex == 14)
	{
		CurrentIndex = 11;
	}
	ABLOG(Warning, TEXT("CurrentIndex %d"), CurrentIndex);
}

void UIBCharacterSelecterWidget::OnNextClicked()
{
	if (!bRightFirstClicked)
	{
		bLeftFirstClicked = true;
		bRightFirstClicked = true;
		CurrentIndex = 11;
		ABLOG(Warning, TEXT("CurrentIndex %d"), CurrentIndex);
		return;
	}
	if (CurrentIndex == 14)
	{
		CurrentIndex = 11;
	}
	else if (CurrentIndex == 11)
	{
		CurrentIndex = 14;
	}
	ABLOG(Warning, TEXT("CurrentIndex %d"), CurrentIndex);
}

void UIBCharacterSelecterWidget::OnConfirmClicked()
{
	bLeftFirstClicked = false;
	bRightFirstClicked = false;

	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	fLevelOpenBufferTime += GetWorld()->GetDeltaSeconds();

	UIBSaveGame* NewPlayerData = NewObject<UIBSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;
	ABLOG(Warning, TEXT("CurrentIndex %d"), CurrentIndex);

	
	auto IBPlayerState = GetDefault<AIBPlayerState>();
	UGameplayStatics::SaveGameToSlot(NewPlayerData, IBPlayerState->SaveSlotName, 0);
	/*if (UGameplayStatics::SaveGameToSlot(NewPlayerData, IBPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartRoom"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}*/

}


