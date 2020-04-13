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
	Montage_Play(LSAttackMontage, 1.0f);
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
	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
		ABCHECK(Montage_IsPlaying(LSAttackMontage));
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), LSAttackMontage);
		break;
	case WeaponType::MIDDLESWORD:
		break;
	}
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
	switch (CurrentAttackMontageType)
	{
	case WeaponType::LONGSWORD:
		return FName(*FString::Printf(TEXT("AttackType%d"), Section));
		break;
	case WeaponType::MIDDLESWORD:
		return FName(*FString::Printf(TEXT("Attack%d"), Section));
		break;
	}
}

void UIBAnimInstance::SetAttackMontageType(WeaponType NewType)
{
	CurrentAttackMontageType = NewType;
}
