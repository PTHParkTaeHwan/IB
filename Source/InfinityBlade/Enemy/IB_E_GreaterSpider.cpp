#include "IB_E_GreaterSpider.h"
#include "IB_E_GreaterSpider_AnimInstance.h"
#include "IBWeapon.h"
#include "IBCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "IBCharacterWidget.h"
#include "IB_E_GREATERSPIDER_AIController.h"
#include "IBCharacterSetting.h"
#include "IBGameInstance.h"
#include "IBPlayerController.h"
#include "IBPlayerState.h"
#include "IBHUDWidget.h"


// Sets default values
AIB_E_GreaterSpider::AIB_E_GreaterSpider()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GREATERSPIDER(TEXT("/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/SK_Greater_Spider.SK_Greater_Spider"));
	if (GREATERSPIDER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GREATERSPIDER.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//�ִϸ��̼� �ν��Ͻ� ��������
	static ConstructorHelpers::FClassFinder<UAnimInstance> GREATERSPIDER_ANIM(TEXT("/Game/dev/Enemy/Animation/GreaterSpiderAnimBlueprint.GreaterSpiderAnimBlueprint_C"));
	if (GREATERSPIDER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GREATERSPIDER_ANIM.Class);
	}

	//�ݸ��� ������ ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IBCharacter"));

	AIControllerClass = AIB_E_GREATERSPIDER_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

	//���� ��� ����
	IsAttacking = false;

	//���� �޺� ����
	MaxCombo = 3;
	AttackEndComboState();


	//���� ���� �����
	AttackRange = 250.0f;
	AttackRadius = 90.0f;

	//�� ���� ����
	SetEnemyMode(EnemyMode::PATROL);

	//Roar
	IsRoar = false;
	IsFirstRoar = false;

	//�ļǸ��
	TentionModeInit();
}

// Called when the game starts or when spawned
void AIB_E_GreaterSpider::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIB_E_GreaterSpider::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	IB_E_GSAnim = Cast<UIB_E_GreaterSpider_AnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != IB_E_GSAnim);
	IB_E_GSAnim->OnMontageEnded.AddDynamic(this, &AIB_E_GreaterSpider::OnAttackMontageEnded);


	IB_E_GSAnim->E_OnNextAttackCheck.AddLambda([this]() -> void {
		if (CharacterInAttackRange)
		{
			AnimNotify_NextAttackCheckOn = true;
		}
	});
	IB_E_GSAnim->E_OnAttackHitCheck.AddUObject(this, &AIB_E_GreaterSpider::AttackCheck);

	//Roar����
	IB_E_GSAnim->E_OnRoarCheck.AddLambda([this]()-> void {
		SetEnemyMode(EnemyMode::TENTIONON);
		IsRoar = false;
	});

	//TentionMode����
	IB_E_GSAnim->E_OnLeftCheck.AddUObject(this, &AIB_E_GreaterSpider::TentionModeInit);
	IB_E_GSAnim->E_OnRightCheck.AddUObject(this, &AIB_E_GreaterSpider::TentionModeInit);
}

float AIB_E_GreaterSpider::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	return 0.0f;
}

// Called every frame
void AIB_E_GreaterSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AIB_E_GreaterSpider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AIB_E_GreaterSpider::SetTentionMode()
{
	if (!IsAttacking)
	{
		IsAttacking = true;
		int32 temp = FMath::RandRange(1, 10);

		if (temp <= 5) IsStayHere = true;
		else IsStayHere = false;

		if (IsStayHere)
		{
			int32 temp2 = FMath::RandRange(1, 10);
			if (temp <= 5)
			{
				AttackOrIdle = true;
			}
			else
			{
				AttackOrIdle = false;
			}
		}
		else if (!IsStayHere)
		{
			int32 temp2 = FMath::RandRange(1, 10);
			if (temp <= 5)
			{
				LeftOrRight = true;
				IB_E_GSAnim->PlayLeftMontage();
			}
			else
			{
				LeftOrRight = false;
				IB_E_GSAnim->PlayRightMontage();
			}
		}
	}

	ABLOG(Warning, TEXT("////////////////////"));
	ABLOG(Warning, TEXT("IsStayHere %d"), IsStayHere);
	ABLOG(Warning, TEXT("AttackOrIdle %d"), AttackOrIdle);
	ABLOG(Warning, TEXT("LeftOrRight %d"), LeftOrRight);
}


void AIB_E_GreaterSpider::Attack()
{
	/*
	0. IsAttacking�� false���� true�� �����ϸ鼭 ���ݸ�� ����.
	1. �������� ���ڸ� �����ؼ�  0-30_��, 31-45_���, 46-55_��, 56-70_��, 71-100_��
	2. �̵� �� ��� �������� 1-3�� �� �ϳ��� ���ڰ� �����ǵ��� �� �� �� ���ڸ�ŭ ��Ÿ�� ���
	3. ���ݸ���� ��� �Ϲ����� ������� ����
	4. ĳ���Ͱ� �Ÿ����� �־����� ���� ���� ��� �ʱ�ȭ
	
	*/
	
	AttackStartComboState();
	IB_E_GSAnim->PlayAttackMontage();
	IB_E_GSAnim->JumpToAttackMontageSection(CurrentCombo);


	/*
	if (!IsAttacking)
	{
		IsAttacking = true;
		int32 temp = FMath::RandRange(1, 10);

		if (0 <= temp && temp <= 100)
		{
			IsLeft = true;
		}
		else if (31 <= temp && temp <= 45)
		{
			ABLOG(Warning, TEXT("IdleOn"));
			IdleOn = true;
		}
		else if (46 <= temp && temp <= 55)
		{
			AttackOn = true;
		}
		else if (56 <= temp && temp <= 70)
		{
			ABLOG(Warning, TEXT("IdleOn"));
			IdleOn = true;
		}
		else
		{
			IsRight = true;
		}
	}*/

	/*if (IsAttacking)
	{
		if (IsLeft)
		{
			IB_E_GSAnim->PlayLeftMontage();
			IsLeft = false;
			MoveLeftOn = true;
			GetActorLocation() + GetActorRightVector() * 50.0f;
		}
		else if (AttackOn)
		{
			AttackStartComboState();
			IB_E_GSAnim->PlayAttackMontage();
			IB_E_GSAnim->JumpToAttackMontageSection(CurrentCombo);
			AttackOn = false;
		}
		else if (IdleOn)
		{
			IdleTime++;
			ABLOG(Warning, TEXT("IdleTime %d"), IdleTime);
			if (IdleTime == 3)
			{
				TentionModeInit();
			}
		}
		else if (IsRight)
		{
			IB_E_GSAnim->PlayRightMontage();
			IsRight = false;
		}
	}*/
}

void AIB_E_GreaterSpider::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{ 
	if (AnimNotify_NextAttackCheckOn)
	{
		AnimNotify_NextAttackCheckOn = false;
		AttackStartComboState();
		IB_E_GSAnim->PlayAttackMontage();
		IB_E_GSAnim->JumpToAttackMontageSection(CurrentCombo);
	}
	else
	{
		IsAttacking = false;
		AttackEndComboState();
		TentionModeInit();
		OnAttackEnd.Broadcast();
	}
}

void AIB_E_GreaterSpider::AttackStartComboState()
{
	//CanNextCombo = true;
	//IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AIB_E_GreaterSpider::AttackEndComboState()
{
	AnimNotify_NextAttackCheckOn = false;
	//IsComboInputOn = false;
	//CanNextCombo = false;
	CurrentCombo = 0;
}

void AIB_E_GreaterSpider::AttackCheck()
{
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool bResults = GetWorld()->SweepMultiByChannel(HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResults ? FColor::Green : FColor::Red;
	float DebugLifeTime = 2.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
	if (bResults)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (CurrentCombo < 3)
			{
				//FDamageEvent DamageEvent;
				//HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			}
			else
			{
				//FDamageEvent DamageEvent;
				//HitResult.Actor->TakeDamage(CharacterStat->GetAttack() * 2, DamageEvent, GetController(), this);
			}
		}
	}
}

bool AIB_E_GreaterSpider::GetIsRoar()
{
	return IsRoar;
}

void AIB_E_GreaterSpider::TentionModeInit()
{
	IsAttacking = false;
	AttackOn = false;

	IsLeft = false;
	MoveLeftOn = false;
	
	IsRight = false;
	MoveRightOn = false;
	
	IdleOn = false;
	IdleTime = 0;

	IsStayHere = false;
	AttackOrIdle = false;
	LeftOrRight = false;
}



bool AIB_E_GreaterSpider::GetIsStayHere()
{
	return IsStayHere;
}

bool AIB_E_GreaterSpider::GetAttackOrIdle()
{
	return AttackOrIdle;
}

bool AIB_E_GreaterSpider::GetLeftOrRight()
{
	return LeftOrRight;
}


void AIB_E_GreaterSpider::SetCharacterInAttackRange(bool InAttackRange)
{
	CharacterInAttackRange = InAttackRange;
}

void AIB_E_GreaterSpider::SetEnemyMode(EnemyMode NewMode)
{
	switch (NewMode)
	{
	case EnemyMode::PATROL:
		break;
	case EnemyMode::TARGETON:
		if (!IsRoar && !IsFirstRoar)
		{
			IsFirstRoar = true;
			IB_E_GSAnim->SetRoarOn(true);
			IB_E_GSAnim->PlayRoarMontage();
			IsRoar = IB_E_GSAnim->GetIsRoar();
		}
		break;
	case EnemyMode::TENTIONON:
		ABLOG(Warning, TEXT("TENTIONON"));
		break;
	case EnemyMode::ATTACK:
		break;
	case EnemyMode::HIT:
		break;
	}
}

