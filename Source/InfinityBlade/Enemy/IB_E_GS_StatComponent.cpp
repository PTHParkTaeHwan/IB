// Fill out your copyright notice in the Description page of Project Settings.

#include "IB_E_GS_StatComponent.h"
#include "IBGameInstance.h"


// Sets default values for this component's properties
UIB_E_GS_StatComponent::UIB_E_GS_StatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	Level = 15;
	// ...
}


// Called when the game starts
void UIB_E_GS_StatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UIB_E_GS_StatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}


// Called every frame
void UIB_E_GS_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIB_E_GS_StatComponent::SetNewLevel(int32 NewLevel)
{
	auto IBGameInstance = Cast<UIBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ABCHECK(nullptr != IBGameInstance);
	CurrentStatData = IBGameInstance->GetIB_E_GSData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		ABLOG(Warning, TEXT("doesn't exist"));
	}
}

void UIB_E_GS_StatComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UIB_E_GS_StatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	E_OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		E_OnHPIsZero.Broadcast();
	}
}

float UIB_E_GS_StatComponent::GetAttack()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float UIB_E_GS_StatComponent::GetHPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

int32 UIB_E_GS_StatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

