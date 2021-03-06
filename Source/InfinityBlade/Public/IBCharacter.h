// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/Character.h"
#include "IBCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);


UCLASS()
class INFINITYBLADE_API AIBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIBCharacter();
	
//캐릭터 스테이트 관리
public:
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	int32 GetExp() const;

private:
	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;
	
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class AIBAIController* IBAIController;

	UPROPERTY()
	class AIBPlayerController* IBPlayerController;
//// end of state Management

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DEFENSE,
		NPC
	};

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const&DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UIBCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;
	
	//공격 모션
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

public:
	bool GetIsRun();
	bool GetIsDefense();
	
	//무기 아이템 습득
	bool CanSetWeapon();
	void SetWeapon(class AIBWeapon* NewWeapon);
		
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AIBWeapon* CurrentWeapon;

private:
	//캐릭터 움직임
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	//카메라 움직임
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	
	//캐릭터 걷기 모션
	void ModeChange();
	void RunChange();
	void ShiftButtonChange();

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;

	FVector ArmLocationTo = FVector::ZeroVector;
	float ArmLocationSpeed = 0.0f;

	FVector CameraLocationTo = FVector::ZeroVector;
	float CameraLocationSpeed = 0.0f;

	bool IsRun;
	bool CurrentShiftButtonOn;
	bool IsDefense;


	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//공격모션
	void AttackStartComboState();
	void AttackEndAttackState();

	//공격 충돌처리
	void AttackCheck();

	//캐릭터 에셋 로드
	void OnAssetLoadCompleted();
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
			
	UPROPERTY()
	class UIBAnimInstance* IBAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

//파티클 시스템
public:
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* FirstHitEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* HitEffect;

	UPROPERTY(VisibleAnywhere, Category = Skill)
	UParticleSystemComponent* SkillEffect_1;

	UPROPERTY(VisibleAnywhere, Category = Skill)
	UParticleSystemComponent* SkillEffect_1_Final;

	UPROPERTY(VisibleAnywhere, Category = Skill)
	UParticleSystemComponent* ShieldSkill;

	UPROPERTY(VisibleAnywhere, Category = Skill)
	UParticleSystemComponent* TestParticle;

	UPROPERTY(VisibleAnywhere, Category = Skill)
	UParticleSystemComponent* TestParticle2;


public:
	void SetHitEffect(AIBWeapon * NewWeapon);


//dead system
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = {};

//AttackStep



private:
	void AttackStepAddLambda();
	void MoveAttackType1();
	void InitAttackStep();

	bool IsAttackType_1Step;
	bool IsAttackType_2Step;

	//AttackMode
private:
	LSAttackMode CurrentAttackMode = LSAttackMode::NONE;
	float CheckIntervalTime;
	bool FirstAttackClick;
	bool SetAttackMode;
	bool TimeCheckStart;

	

	bool SecondAttackClick;
	bool CanComboAttack;
	int32 CurrentBasicAttackSection;

public:
	LSAttackMode GetCurrentAttackMode();


	//skill
public:
	void InitSkillParticle();
	void InitFirstSkill();
	void InitSecondSkill();
	void Skill_3();

private:
	void SkillHub(float DeltaTime);

	//1st
	void InitGroundBurstSkillParameter();
	bool bFirstSkillEffect;
	bool bFirstSkillMontagePlay;
	float EffectIntervalTime;
	int32 EffectNum;

	//2nd
	void InitShieldSkillParameter();
	bool bSecondSkillEffect;
	float ShieldSkillActiveTime;


	FVector SkillStartLocation = FVector::ZeroVector;
	FVector SkillStartForwardVector = FVector::ZeroVector;
	//테스트용 Parameter
private:
	void TestParameter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testparameter, Meta = (AllowPrivateAccess = true))
	float TestFloat1;//거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testparameter, Meta = (AllowPrivateAccess = true))
	float TestFloat2; //스킬 발동 시간 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testparameter, Meta = (AllowPrivateAccess = true))
	int32 TestInt1;
};
