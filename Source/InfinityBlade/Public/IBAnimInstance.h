// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Animation/AnimInstance.h"
#include "IBAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

DECLARE_MULTICAST_DELEGATE(FOnAttackType1_1StepStartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackType1_1StepDoneCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackType1_2StepStartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackType1_2StepDoneCheckDelegate);

DECLARE_MULTICAST_DELEGATE(FOnFirstSkillStartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSecondSkillDoneCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnForthSkillStartCheckDelegate);


/**
 * 
 */


UCLASS()
class INFINITYBLADE_API UIBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	UIBAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void StopAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	//LS BASIC 
	void PlayBasicAttackNontage();
	void StopBasicAttackMontage();
	void JumpToBasicAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

	//공격모션
public:
	FOnAttackType1_1StepStartCheckDelegate FOnAttackType1_1StepStartCheck;
	FOnAttackType1_1StepDoneCheckDelegate FOnAttackType1_1StepDoneCheck;
	FOnAttackType1_2StepStartCheckDelegate FOnAttackType1_2StepStartCheck;
	FOnAttackType1_2StepDoneCheckDelegate FOnAttackType1_2StepDoneCheck;
	FOnSecondSkillDoneCheckDelegate FOnSecondSkillDoneCheck;
	FOnForthSkillStartCheckDelegate FOnForthSkillStartCheck;

private:
	//AttackType1
	UFUNCTION()
	void AnimNotify_AttackType1_1StepStart();

	UFUNCTION()
	void AnimNotify_AttackType1_1StepDone();

	UFUNCTION()
	void AnimNotify_AttackType1_2StepStart();

	UFUNCTION()
	void AnimNotify_AttackType1_2StepDone();

	UFUNCTION()
	void AnimNotify_FirstSkillStart();

	UFUNCTION()
	void AnimNotify_ShieldSkillDone();

	UFUNCTION()
	void AnimNotify_UltimateSkillStart();



private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDefense;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* LSAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* LSBasicAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ShieldMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltimateSkill;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;


private:
	WeaponType CurrentAttackMontageType = WeaponType::LONGSWORD;

public:
	void SetAttackMontageType(WeaponType NewType);

	//스킬

public:
	void PlayFirstSkillMontage(int32 SectionNum);
	FOnFirstSkillStartCheckDelegate FOnFirstSkillStartCheck;

	void PlayShieldSkillMontage();
	void PlayUltimateSkillMontage();
};
