// Fill out your copyright notice in the Description page of Project Settings.

#include "IB_E_GreaterSpider_AnimInstance.h"

UIB_E_GreaterSpider_AnimInstance::UIB_E_GreaterSpider_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
	RoarOn = false;
	
	//Anim moatage init
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/dev/Enemy/Animation/SK_Greater_Spider_Skeleton_Montage.SK_Greater_Spider_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		E_AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROAR_ASSET(TEXT("/Game/dev/Enemy/Animation/SK_Greater_Spider_Skeleton_Montage_Roar.SK_Greater_Spider_Skeleton_Montage_Roar"));
	if (ROAR_ASSET.Succeeded())
	{
		E_Roar_Asset = ROAR_ASSET.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> LEFT_MONTAGE(TEXT("/Game/dev/Enemy/Animation/SK_Greater_Spider_Skeleton_Montage_Left.SK_Greater_Spider_Skeleton_Montage_Left"));
	if (LEFT_MONTAGE.Succeeded())
	{
		E_LeftMontage = LEFT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RIGHT_MONTAGE(TEXT("/Game/dev/Enemy/Animation/SK_Greater_Spider_Skeleton_Montage_Right.SK_Greater_Spider_Skeleton_Montage_Right"));
	if (RIGHT_MONTAGE.Succeeded())
	{
		E_RightMontage = RIGHT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_MONTAGE(TEXT("/Game/dev/Enemy/Animation/SK_Greater_Spider_Skeleton_Montage_Hit.SK_Greater_Spider_Skeleton_Montage_Hit"));
	if (HIT_MONTAGE.Succeeded())
	{
		E_HitMontage = HIT_MONTAGE.Object;
	}


}

void UIB_E_GreaterSpider_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;
	
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
	
}

void UIB_E_GreaterSpider_AnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	ABLOG(Warning, TEXT("PlayAttackMontage"));
	Montage_Play(E_AttackMontage, 1.0f);
}

void UIB_E_GreaterSpider_AnimInstance::StopAttackMontage()
{
	Montage_Stop(0.1f, E_AttackMontage);
}

void UIB_E_GreaterSpider_AnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(E_AttackMontage));
	//Montage_Play(E_AttackMontage, 1.0f);
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), E_AttackMontage);
}

void UIB_E_GreaterSpider_AnimInstance::JumpToAttackMontageSectionInCombo(int32 NewSection)
{
	ABCHECK(!IsDead);
	//ABCHECK(Montage_IsPlaying(E_AttackMontage));
	Montage_Play(E_AttackMontage, 1.0f);
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), E_AttackMontage);
}


void UIB_E_GreaterSpider_AnimInstance::AnimNotify_AttackHitCheck()
{
	E_OnAttackHitCheck.Broadcast();
}

void UIB_E_GreaterSpider_AnimInstance::AnimNotify_NextAttackCheck()
{
	E_OnNextAttackCheck.Broadcast();
	/*
	1. 노티파이로 함수가 불러졌을 때
	2. 플레이어가 범위 안에 있다면
	3. 다음 몽타주 섹션으로 이동

	*/
}

void UIB_E_GreaterSpider_AnimInstance::PlayRoarMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(E_Roar_Asset, 1.0f);
	RoarOn = true;
}
void UIB_E_GreaterSpider_AnimInstance::SetRoarOn(bool NewRoar)
{
	RoarOn = NewRoar;
}
bool UIB_E_GreaterSpider_AnimInstance::GetIsRoar()
{
	return RoarOn;
}
void UIB_E_GreaterSpider_AnimInstance::AnimNotify_Roar()
{
	Montage_Stop(0.1f, E_Roar_Asset);
	RoarOn = false;
	E_OnRoarCheck.Broadcast();
}

FName UIB_E_GreaterSpider_AnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}


void UIB_E_GreaterSpider_AnimInstance::PlayLeftMontage()
{
	ABLOG(Warning, TEXT("PlayLeftMontage"));
	Montage_Play(E_LeftMontage, 0.5f);
}
void UIB_E_GreaterSpider_AnimInstance::AnimNotify_LeftDone()
{
	ABLOG(Warning, TEXT("AnimNotify_LeftDone"));
	E_OnLeftCheck.Broadcast();
}

void UIB_E_GreaterSpider_AnimInstance::PlayRightMontage()
{
	ABLOG(Warning, TEXT("PlayRightMontage"));
	Montage_Play(E_RightMontage, 0.5f);
}
void UIB_E_GreaterSpider_AnimInstance::AnimNotify_RightDone()
{
	ABLOG(Warning, TEXT("AnimNotify_RightDone"));
	E_OnRightCheck.Broadcast();
}

void UIB_E_GreaterSpider_AnimInstance::PlayHitMontage()
{
	ABLOG(Warning, TEXT("PlayHitMontage"));
	Montage_Play(E_HitMontage, 1.0f);
}
void UIB_E_GreaterSpider_AnimInstance::AnimNotify_Hit()
{
	ABLOG(Warning, TEXT("AnimNotify_Hit"));
	E_OnHitCheck.Broadcast();
}

void UIB_E_GreaterSpider_AnimInstance::StopAllMontage()
{
	ABLOG(Warning, TEXT("StopAllMontage"));
	ABLOG(Warning, TEXT("E_AttackMontage %d"), Montage_IsPlaying(E_AttackMontage));
	ABLOG(Warning, TEXT("E_LeftMontage %d"), Montage_IsPlaying(E_LeftMontage));
	ABLOG(Warning, TEXT("E_RightMontage %d"), Montage_IsPlaying(E_RightMontage));

	if (Montage_IsPlaying(E_AttackMontage))
	{
		Montage_Stop(0.1f, E_AttackMontage);
	}
	else if (Montage_IsPlaying(E_LeftMontage))
	{
		Montage_Stop(0.1f, E_LeftMontage);
	}
	else if (Montage_IsPlaying(E_RightMontage))
	{
		Montage_Stop(0.1f, E_RightMontage);
	}
}