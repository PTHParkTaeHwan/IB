#include "IB_E_GreaterSpider.h"
#include "IB_E_GreaterSpider_AnimInstance.h"
#include "IBWeapon.h"
#include "IB_E_GS_StatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "IB_E_GS_Widget.h"
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

	//애니메이션 인스턴스 가져오기
	static ConstructorHelpers::FClassFinder<UAnimInstance> GREATERSPIDER_ANIM(TEXT("/Game/dev/Enemy/Animation/GreaterSpiderAnimBlueprint.GreaterSpiderAnimBlueprint_C"));
	if (GREATERSPIDER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GREATERSPIDER_ANIM.Class);
	}

	//캐릭터스텟컴퍼넌트 가져오기
	CharacterStat = CreateDefaultSubobject<UIB_E_GS_StatComponent>(TEXT("CHARACTERSTAT"));

	//HPBar Widget
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/dev/Enemy/UI/UI_E_HPUIBar.UI_E_HPUIBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(75.0f, 25.0f));
	}
	HPBarWidget->SetHiddenInGame(true);	


	//콜리전 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IBCharacter"));

	AIControllerClass = AIB_E_GREATERSPIDER_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

	//공격 모션 관리
	IsAttacking = false;

	//공격 콤보 관리
	MaxCombo = 3;


	//공격 범위 디버그
	AttackRange = 250.0f;
	AttackRadius = 90.0f;

	//적 상태 관리
	SetEnemyMode(EnemyMode::PATROL);

	//Roar
	IsRoar = false;
	IsFirstRoar = false;

	//탠션모드
	TentionModeInit();

	//Dead State
	DeadModeOn = false;

	//hit particle
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));
	HitEffect->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HIT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_ImpactSpark.P_ImpactSpark"));
	if (P_HIT.Succeeded())
	{
		HitEffect->SetTemplate(P_HIT.Object);
		HitEffect->bAutoActivate = false;
	}

	HitMotionOn = false;

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

	//Roar관리
	IB_E_GSAnim->E_OnRoarCheck.AddLambda([this]()-> void {
		SetEnemyMode(EnemyMode::TENTIONON);
		IsRoar = false;
	});

	//TentionMode관리
	IB_E_GSAnim->E_OnLeftCheck.AddUObject(this, &AIB_E_GreaterSpider::TentionModeInit);
	IB_E_GSAnim->E_OnRightCheck.AddUObject(this, &AIB_E_GreaterSpider::TentionModeInit);

	CharacterStat->E_OnHPIsZero.AddLambda([this]()-> void {
		DeadModeOn = true;
	});

	//HP위젯 연결
	auto CharacterWidget = Cast<UIB_E_GS_Widget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	//hit motion
	IB_E_GSAnim->E_OnHitCheck.AddLambda([this]()->void {
		HitMotionOn = false;
		TentionModeInit();
	});
}

float AIB_E_GreaterSpider::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	ABLOG(Warning, TEXT("TakeDamage"));
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//IB_E_GSAnim->StopAllMontage();
	HitMotionOn = true;	
	HitEffect->Activate(true);
	IB_E_GSAnim->PlayHitMontage();
	CharacterStat->SetDamage(FinalDamage);

	if (DeadModeOn)
	{
		auto IBPlayerController = Cast<AIBPlayerController>(EventInstigator);
		ABCHECK(nullptr != IBPlayerController, 0.0f);
		IBPlayerController->NPCKill(this);
		Destroy(this);
	}
	return FinalDamage;
}

// Called every frame
void AIB_E_GreaterSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IdleOn)
	{
		IdleTime += DeltaTime;
		if (IdleTime >= 2.0f)
		{
			TentionModeInit();
		}
	}
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
			if (temp2 <= 7)
			{
				AttackOrIdle = true;
			}
			else
			{
				AttackOrIdle = false;
				IdleOn = true;
			}
		}
		else 
		{
			int32 temp3 = FMath::RandRange(1, 10);
			if (temp3 <= 5)
			{
				LeftOrRight = true;
			}
			else
			{
				LeftOrRight = false;
			}
		}
	}
}


void AIB_E_GreaterSpider::Attack()
{
	if (!AttackOn && !HitMotionOn)
	{
		AttackOn = true;
		AttackStartComboState();
		ABLOG(Warning, TEXT("Attack()"));
		IB_E_GSAnim->PlayAttackMontage();
		IB_E_GSAnim->JumpToAttackMontageSection(CurrentCombo);
	}
}

void AIB_E_GreaterSpider::SetHPBarWidgetHiddenInGame(bool NewStat)
{
	HPBarWidget->SetHiddenInGame(NewStat);
}

void AIB_E_GreaterSpider::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{ 
	ABLOG(Warning, TEXT("OnAttackMontageEnded(%s, %d)"), *Montage->GetName(), bInterrupted);
	if (AnimNotify_NextAttackCheckOn)
	{
		AnimNotify_NextAttackCheckOn = false;
		AttackStartComboState();
		ABLOG(Warning, TEXT("OnAttackMontageEnded(%s, %d), if (AnimNotify_NextAttackCheckOn)"), *Montage->GetName(), bInterrupted);
		IB_E_GSAnim->PlayAttackMontage();
		IB_E_GSAnim->JumpToAttackMontageSection(CurrentCombo);
	}
	else if(!AnimNotify_NextAttackCheckOn)
	{
		TentionModeInit();
		IsAttacking = false;
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
			if (CurrentCombo < 2)
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			}
			else
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(CharacterStat->GetAttack() * 2, DamageEvent, GetController(), this);
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
	AnimNotify_NextAttackCheckOn = false;
	CurrentCombo = 0;

	//IsAttacking = false;
	AttackOn = false;
	IsLeftOrRightMove = false;

	IdleOn = false;
	IdleTime = 0;

	IsStayHere = false;
	AttackOrIdle = false;
	LeftOrRight = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

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

bool AIB_E_GreaterSpider::GetIsAttacking()
{
	return IsAttacking;
}

void AIB_E_GreaterSpider::SetIsAttacking(bool NewState)
{
	IsAttacking = NewState;
}

bool AIB_E_GreaterSpider::GetAttackOn()
{
	return AttackOn;
}

void AIB_E_GreaterSpider::SetbOrientRotationToMovement(bool NewRotation)
{
	GetCharacterMovement()->bOrientRotationToMovement = NewRotation;
}

void AIB_E_GreaterSpider::PlayLeftOrRightMontage()
{
	if (LeftOrRight && !HitMotionOn)
	{
		IB_E_GSAnim->PlayLeftMontage();
		IsLeftOrRightMove = true;
	}
	else if (!LeftOrRight && !HitMotionOn)
	{
		IB_E_GSAnim->PlayRightMontage();
		IsLeftOrRightMove = true;
	}
}

bool AIB_E_GreaterSpider::GetIsLeftOrRightMove()
{
	return IsLeftOrRightMove;
}

int32 AIB_E_GreaterSpider::GetExp() const
{
	return CharacterStat->GetDropExp();
}

bool AIB_E_GreaterSpider::GetHitMotionOn()
{
	return HitMotionOn;
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
		break;
	case EnemyMode::ATTACK:
		break;
	case EnemyMode::HIT:
		break;
	}
}

