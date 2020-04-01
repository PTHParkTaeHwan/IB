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
	ABLOG(Warning, TEXT("NextCharacter"));
	bForward ? CurrentIndex++ : CurrentIndex--;
	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<UIBCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto IBGameInstance = GetWorld()->GetGameInstance<UIBGameInstance>();
	ABCHECK(nullptr != IBGameInstance);
	ABCHECK(TargetComponent.IsValid());


	USkeletalMesh* Asset = IBGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
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
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

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
}

void UIBCharacterSelecterWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UIBCharacterSelecterWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UIBCharacterSelecterWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	UIBSaveGame* NewPlayerData = NewObject<UIBSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	
	auto IBPlayerState = GetDefault<AIBPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, IBPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartRoom"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}


