// Fill out your copyright notice in the Description page of Project Settings.

#include "IBAnimInstance.h"
#include "IBCharacter.h"


UIBAnimInstance::UIBAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	IsDefense = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> LSATTACK_MONTAGE(TEXT("/Game/Book/Animations/Attack_Montage/LongSword_AttackMontage_Type1.LongSword_AttackMontage_Type1"));
	if (LSATTACK_MONTAGE.Succeeded())
	{
		LSAttackMontage = LSATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> LSBASICATTACK_MONTAGE(TEXT("/Game/Book/Animations/Attack_Montage/LongSword_AttackMontage_BasicAttack.LongSword_AttackMontage_BasicAttack"));
	if (LSBASICATTACK_MONTAGE.Succeeded())
	{
		LSBasicAttackMontage = LSBASICATTACK_MONTAGE.Object;
	}


}

void UIBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		//Pawn->GetMovementComponent()->IsFalling();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
		auto testCh = Cast<AIBCharacter>(Pawn);
		if (testCh)
		{			
			IsRun = testCh->GetIsRun();
			IsDefense = testCh->GetIsDefense();
		}
	}
}

void UIBAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
	Montage_Play(LSAttackMontage, 1.2f);
		break;
	case WeaponType::MIDDLESWORD:
		break;
	}
}

void UIBAnimInstance::StopAttackMontage()
{
	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
		Montage_Stop(0.1f, LSAttackMontage);
		break;
	case WeaponType::MIDDLESWORD:
		break;
	}
}

void UIBAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	auto Pawn = TryGetPawnOwner();
	auto Player = Cast<AIBCharacter>(Pawn);

	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
		if (Player->GetCurrentAttackMode() == LSAttackMode::BASIC)
		{
			Montage_JumpToSection(GetAttackMontageSectionName(NewSection), LSBasicAttackMontage);
		}
		else if (Player->GetCurrentAttackMode() == LSAttackMode::COMBO)
		{
			Montage_JumpToSection(GetAttackMontageSectionName(NewSection), LSAttackMontage);
		}
		break;
	case WeaponType::MIDDLESWORD:
		break;
	}
}

void UIBAnimInstance::PlayBasicAttackNontage()
{
	Montage_Play(LSBasicAttackMontage, 1.4f);
}

void UIBAnimInstance::StopBasicAttackMontage()
{
	Montage_Stop(0.2f, LSBasicAttackMontage);
}

void UIBAnimInstance::JumpToBasicAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), LSBasicAttackMontage);
}

void UIBAnimInstance::AnimNotify_AttackHitCheck()
{
	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
		OnAttackHitCheck.Broadcast();
		break;
	case WeaponType::MIDDLESWORD:
		break;
	}
}

void UIBAnimInstance::AnimNotify_NextAttackCheck()
{
	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
		OnNextAttackCheck.Broadcast();
		break;
	case WeaponType::MIDDLESWORD:
		break;
	}
}

FName UIBAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);

	auto Pawn = TryGetPawnOwner();
	auto Player = Cast<AIBCharacter>(Pawn);

	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
		if (Player->GetCurrentAttackMode() == LSAttackMode::BASIC)
		{
			ABLOG(Warning, TEXT("Attack%d"), Section);
			return FName(*FString::Printf(TEXT("Attack%d"), Section));
		}
		else if (Player->GetCurrentAttackMode() == LSAttackMode::COMBO)
		{
			return FName(*FString::Printf(TEXT("AttackType%d"), Section));
		}
		break;
	case WeaponType::MIDDLESWORD:
		return FName(*FString::Printf(TEXT("Attack%d"), Section));
		break;
	}
	return FName(*FString::Printf(TEXT("AttackType%d"), Section));
}

void UIBAnimInstance::AnimNotify_AttackType1_1StepStart()
{
	FOnAttackType1_1StepStartCheck.Broadcast();
}

void UIBAnimInstance::AnimNotify_AttackType1_1StepDone()
{
	FOnAttackType1_1StepDoneCheck.Broadcast();
}

void UIBAnimInstance::AnimNotify_AttackType1_2StepStart()
{
	FOnAttackType1_2StepStartCheck.Broadcast();
}

void UIBAnimInstance::AnimNotify_AttackType1_2StepDone()
{
	FOnAttackType1_2StepDoneCheck.Broadcast();
}

void UIBAnimInstance::AnimNotify_FirstSkillStart()
{
	ABLOG(Warning, TEXT("AnimNotify_FirstSkillStart"));
	FOnFirstSkillStartCheck.Broadcast();
}

void UIBAnimInstance::SetAttackMontageType(WeaponType NewType)
{
	CurrentAttackMontageType = NewType;
}

void UIBAnimInstance::PlayFirstSkillMontage(int32 SectionNum)
{
	ABLOG(Warning, TEXT("PlayFirstSkillMontage"));
	Montage_Play(AttackMontage, 1.0f);
	Montage_JumpToSection(FName(*FString::Printf(TEXT("Attack4"))), AttackMontage);
	
}
