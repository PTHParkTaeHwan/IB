// Fill out your copyright notice in the Description page of Project Settings.

#include "IBCharacter.h"
#include "IBAnimInstance.h"
#include "IBWeapon.h"
#include "IBCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "IBCharacterWidget.h"
#include "IBAIController.h"
#include "IBCharacterSetting.h"
#include "IBGameInstance.h"
#include "IBPlayerController.h"
#include "IBPlayerState.h"
#include "IBHUDWidget.h"


// Sets default values
AIBCharacter::AIBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Pawn에 캡슐, 메쉬, 무브먼트를 가져올 수 있는 함수가 있기 때문에 스프링 암과 카메라만 따로 만들어준다.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UIBCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CardBoard(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Bladed.SK_CharM_Bladed"));
	if (CardBoard.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CardBoard.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//애니메이션 인스턴스 가져오기
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	//캐릭 생성되면서 무기 부착하기
	/*FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword22/SK_Blade_HeroSword22.SK_Blade_HeroSword22"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}*/

	//카메라 모드 전환
	SetControlMode(EControlMode::GTA);
	ArmLengthSpeed = 3.0f;
	ArmLocationSpeed = 5.0f;
	CameraLocationSpeed = 1.5f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//걷기 모드 전환
	IsRun = false;
	CurrentShiftButtonOn = false;
	GetCharacterMovement()->MaxWalkSpeed = 400;
	
	

	//공격 모션 관리
	IsAttacking = false;

	//공격 콤보 관리
	MaxCombo = 4;
	AttackEndComboState();

	//콜리전 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IBCharacter"));

	//공격 범위 디버그
	AttackRange = 250.0f;
	AttackRadius = 90.0f;

	//파티클 시스템
	FirstHitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));
	FirstHitEffect->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HIT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_ImpactSpark.P_ImpactSpark"));
	if (P_HIT.Succeeded())
	{
		FirstHitEffect->SetTemplate(P_HIT.Object);
		FirstHitEffect->bAutoActivate = false;
	}

	//HP, SE UI
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPUIBar.UI_HPUIBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	AIControllerClass = AIBAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	auto DefaultSetting = GetDefault<UIBCharacterSetting>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}

	//State Management
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	bCanBeDamaged = false;

	//dead System
	DeadTimer = 3.0f;
}

void AIBCharacter::SetCharacterState(ECharacterState NewState)
{
	ABCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
		if (bIsPlayer)
		{
			DisableInput(IBPlayerController);

			IBPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			auto IBPlayerState = Cast<AIBPlayerState>(PlayerState);
			ABCHECK(nullptr != IBPlayerState);
			CharacterStat->SetNewLevel(IBPlayerState->GetCharacterLevel());
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;
		break;
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		bCanBeDamaged = true;
		CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
			SetCharacterState(ECharacterState::DEAD);
		});

		auto CharacterWidget = Cast<UIBCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		ABCHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::GTA);
			EnableInput(IBPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			IBAIController->RunAI();
		}

		break; 
	}
	case ECharacterState::DEAD:
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		IBAnim->SetDeadAnim();
		bCanBeDamaged = false;

		if (bIsPlayer)
		{
			DisableInput(IBPlayerController);
		}
		else
		{
			IBAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() ->void {
			if (bIsPlayer)
			{
				//AIBPlayerController->RestartLevel();
				ABLOG(Warning, TEXT("RestartLevel"));
			}
			else
			{
				Destroy();
			}
		}), DeadTimer, false);
		break;
	}
}

ECharacterState AIBCharacter::GetCharacterState() const
{
	return CurrentState;
}

int32 AIBCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

// Called when the game starts or when spawned
void AIBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bIsPlayer = IsPlayerControlled();

	if (bIsPlayer)
	{
		IBPlayerController = Cast<AIBPlayerController>(GetController());
		ABCHECK(nullptr != IBPlayerController);
	}
	else
	{
		IBAIController = Cast<AIBAIController>(GetController());
		ABCHECK(nullptr != IBAIController);
	}
	
	auto DefaultSetting = GetDefault<UIBCharacterSetting>();
	
	if (bIsPlayer)
	{
		auto IBPlayerState = Cast<AIBPlayerState>(PlayerState);
		ABCHECK(nullptr != IBPlayerState);
		AssetIndex = IBPlayerState->GetCharacterIndex();
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}
	
	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto IBGameInstance = Cast<UIBGameInstance>(GetGameInstance());
	ABCHECK(nullptr != IBGameInstance);
	AssetStreamingHandle = IBGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AIBCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);
}

void AIBCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;
	
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		IsDefense = false;
		ArmLengthTo = 450.0f;
		CameraLocationTo = FVector(0.0f, 0.0f, 0.0f);
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DEFENSE:
		IsDefense = true;
		ArmLengthTo = 180.0f;
		CameraLocationTo = FVector(0.0f, -30.0f, 50.0f);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// Called every frame
void AIBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, CameraLocationTo, DeltaTime, CameraLocationSpeed);

	RunChange();

	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::DEFENSE:
		break;
	}
	//ABLOG(Warning, TEXT("%d"), GetCharacterMovement()->);
	
}

void AIBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	IBAnim = Cast<UIBAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != IBAnim);
	//매크로를 활용해 애님 인스턴스의 OnMontageEnded 델리게이트와 우리가 선언한 OnAttackMontageEnded를 연결한다.
	IBAnim->OnMontageEnded.AddDynamic(this, &AIBCharacter::OnAttackMontageEnded);

	IBAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			IBAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	IBAnim->OnAttackHitCheck.AddUObject(this, &AIBCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		IBAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});

	auto CharacterWidget = Cast<UIBCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

}

float AIBCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FirstHitEffect->Activate(true);
	
	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
		CharacterStat->SetDamage(FinalDamage, false);
		break;
	case AIBCharacter::EControlMode::DEFENSE:
		CharacterStat->SetDamage(FinalDamage, true);
		break;
	}
	if (CurrentControlMode == AIBCharacter::EControlMode::NPC)
	{
		CharacterStat->SetDamage(FinalDamage, false);
	}

	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto IBPlayerController = Cast<AIBPlayerController>(EventInstigator);
			ABCHECK(nullptr != IBPlayerController, 0.0f);
			IBPlayerController->NPCKill(this);
		}
	}
	return FinalDamage;
}

//void AIBCharacter::PossessedBy(AController * NewController)
//{
//	Super::PossessedBy(NewController);
//	if (IsPlayerControlled())
//	{
//		SetControlMode(EControlMode::GTA);
//	}
//	else
//	{
//		SetControlMode(EControlMode::NPC);
//		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
//	}
//}

// Called to bind functionality to input
void AIBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AIBCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AIBCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AIBCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AIBCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("DefenseMode"), EInputEvent::IE_Pressed, this, &AIBCharacter::ModeChange);
	PlayerInputComponent->BindAction(TEXT("DefenseMode"), EInputEvent::IE_Released, this, &AIBCharacter::ModeChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AIBCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Runing"), EInputEvent::IE_Pressed, this, &AIBCharacter::ShiftButtonChange);
	PlayerInputComponent->BindAction(TEXT("Runing"), EInputEvent::IE_Released, this, &AIBCharacter::ShiftButtonChange);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AIBCharacter::Attack);

}
bool AIBCharacter::GetIsRun()
{
	return IsRun;
}
bool AIBCharacter::GetIsDefense()
{
	return IsDefense;
}
bool AIBCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}
void AIBCharacter::SetWeapon(AIBWeapon * NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}

}
void AIBCharacter::UpDown(float NewAxisValue)
{
	if(!IsAttacking) AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
	else AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue/100);
}
void AIBCharacter::LeftRight(float NewAxisValue)
{
	if (!IsAttacking) AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
	else  AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue/100);

	

}
void AIBCharacter::LookUp(float NewAxisValue)
{
	//Y축
	AddControllerPitchInput(NewAxisValue);
}
void AIBCharacter::Turn(float NewAxisValue)
{
	//Z축
	AddControllerYawInput(NewAxisValue);
}
void AIBCharacter::ModeChange()
{
	if (CurrentControlMode == EControlMode::GTA)
	{
		SetControlMode(EControlMode::DEFENSE);
	}
	else if (CurrentControlMode == EControlMode::DEFENSE)
	{
		SetControlMode(EControlMode::GTA);
	}
}

void AIBCharacter::RunChange()
{
	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::GTA:
		if (CurrentShiftButtonOn && this->GetVelocity().Size() > 0)
		{
			IsRun = true;
			GetCharacterMovement()->MaxWalkSpeed = 600;
		}
		else if (!CurrentShiftButtonOn && this->GetVelocity().Size() > 0)
		{
			IsRun = false;
			GetCharacterMovement()->MaxWalkSpeed = 400;
		}
		else
		{
			IsRun = false;
			GetCharacterMovement()->MaxWalkSpeed = 400;
		}
		break;
	case AIBCharacter::EControlMode::DEFENSE:
		GetCharacterMovement()->MaxWalkSpeed = 200;
		break;
	}
	
}

void AIBCharacter::ShiftButtonChange()
{
	if (!CurrentShiftButtonOn)
	{
		CurrentShiftButtonOn = true;
		IsAttacking = false;
		AttackEndComboState();
		IBAnim->StopAttackMontage();
	}
	else if (CurrentShiftButtonOn)
	{
		CurrentShiftButtonOn = false;
	}
}

void AIBCharacter::Attack()
{
	//////
	/*
	float temp = 200.0f;
	FVector TraceVec = GetActorRightVector();
	FVector Center = GetActorLocation() + GetActorRightVector() * temp;
	float HalfHeight = 50.0f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = FColor::Green;
	float DebugLifeTime = 2.0f;

	GetActorLocation() + GetActorRightVector() * temp;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

	
	float temp = 200.0f;
	FVector TraceVec = GetActorForwardVector() * temp;
	FVector Center = GetActorLocation() + GetActorRightVector() * temp;
	float HalfHeight = 50.0f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = FColor::Green;
	float DebugLifeTime = 2.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);*/

	switch (CurrentControlMode)
	{
	case AIBCharacter::EControlMode::NPC:
		if (IsAttacking)
		{
			ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
			if (CanNextCombo)
			{
				IsComboInputOn = true;
			}
		}
		else
		{
			ABCHECK(CurrentCombo == 0);
			AttackStartComboState();
			IBAnim->PlayAttackMontage();
			IBAnim->JumpToAttackMontageSection(CurrentCombo);
			IsAttacking = true;
		}
		break;
	case AIBCharacter::EControlMode::GTA:
		if (IsAttacking)
		{
			ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
			if (CanNextCombo)
			{
				IsComboInputOn = true;
			}
		}
		else
		{
			ABCHECK(CurrentCombo == 0);
			AttackStartComboState();
			IBAnim->PlayAttackMontage();
			IBAnim->JumpToAttackMontageSection(CurrentCombo);
			IsAttacking = true;
		}
		break;
	}
}

void AIBCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void AIBCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AIBCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AIBCharacter::AttackCheck()
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
			if (CurrentCombo < 4)
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			}
			else
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(CharacterStat->GetAttack()*2, DamageEvent, GetController(), this);
			}
		}
	}
}

void AIBCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);
	
	SetCharacterState(ECharacterState::READY);

}




