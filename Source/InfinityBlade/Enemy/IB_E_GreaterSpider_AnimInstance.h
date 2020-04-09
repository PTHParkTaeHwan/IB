// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Animation/AnimInstance.h"
#include "IB_E_GreaterSpider_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(E_FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(E_FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(E_FOnRoarCheckDelegate);
DECLARE_MULTICAST_DELEGATE(E_FOnLeftCheckDelegate);
DECLARE_MULTICAST_DELEGATE(E_FOnRightCheckDelegate);
DECLARE_MULTICAST_DELEGATE(E_FOnHitCheckDelegate);



/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIB_E_GreaterSpider_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UIB_E_GreaterSpider_AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//공격 모션 관리 함수
	void PlayAttackMontage();
	void StopAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	void JumpToAttackMontageSectionInCombo(int32 NewSection);
	
	//전투시작 함성 관리 함수
	void PlayRoarMontage();
	void SetRoarOn(bool NewRoar);
	bool GetIsRoar();
	
	//TentionMode 관련
	void PlayLeftMontage();
	void PlayRightMontage();

	//HIT MOTION
	void PlayHitMontage();
	
	void StopAllMontage();


public:
	E_FOnNextAttackCheckDelegate E_OnNextAttackCheck;
	E_FOnAttackHitCheckDelegate E_OnAttackHitCheck;
	E_FOnRoarCheckDelegate E_OnRoarCheck;
	E_FOnLeftCheckDelegate E_OnLeftCheck;
	E_FOnRightCheckDelegate E_OnRightCheck;
	E_FOnHitCheckDelegate E_OnHitCheck;

	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_Roar();

	UFUNCTION()
	void AnimNotify_LeftDone();

	UFUNCTION()
	void AnimNotify_RightDone();

	UFUNCTION()
	void AnimNotify_Hit();


	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsRun;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* E_AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* E_Roar_Asset;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* E_LeftMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* E_RightMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* E_HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool RoarOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool HitMotionOn;

	
};
