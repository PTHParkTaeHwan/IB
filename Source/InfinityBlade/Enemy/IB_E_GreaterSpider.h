// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/Character.h"
#include "IB_E_GreaterSpider.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class INFINITYBLADE_API AIB_E_GreaterSpider : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIB_E_GreaterSpider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const&DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//공격 모션
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	//콤보 진행 유무
	void SetCharacterInAttackRange(bool InAttackRange);

	void SetEnemyMode(EnemyMode NewMode);

	//HP Bar widget 관리
public:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UIB_E_GS_StatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	void SetHPBarWidgetHiddenInGame(bool NewStat);
	

private:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//공격모션
	void AttackStartComboState();
	void AttackEndComboState();

	//공격 충돌처리
	void AttackCheck();


private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	/*UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY()
		class UIB_E_GreaterSpider_AnimInstance* IB_E_GSAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CharacterInAttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool AnimNotify_NextAttackCheckOn;


	//roar 구현
private:
	bool IsRoar;
	bool IsFirstRoar;
public:
	bool GetIsRoar();

	//tentnion mode 구현
private:
	bool AttackOn;
	bool IsLeft;
	bool MoveLeftOn;
	bool IsRight;
	bool MoveRightOn;
	bool IdleOn;
	float IdleTime;

	bool IsStayHere;
	bool AttackOrIdle;
	bool LeftOrRight;

public:
	void TentionModeInit();
	void SetTentionMode();
	bool GetIsStayHere();
	bool GetAttackOrIdle();
	bool GetLeftOrRight();
	bool GetIsAttacking();
	void SetbOrientRotationToMovement(bool NewRotation);

	//dead State
private:
	bool DeadModeOn;

public:
	int32 GetExp() const;

};
