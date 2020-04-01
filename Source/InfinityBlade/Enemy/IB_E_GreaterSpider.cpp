// Fill out your copyright notice in the Description page of Project Settings.

#include "IB_E_GreaterSpider.h"
#include "IBAnimInstance.h"
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

	//애니메이션 인스턴스 가져오기
	static ConstructorHelpers::FClassFinder<UAnimInstance> GREATERSPIDER_ANIM(TEXT("/Game/dev/Enemy/Animation/GreaterSpiderAnimBlueprint.GreaterSpiderAnimBlueprint_C"));
	if (GREATERSPIDER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GREATERSPIDER_ANIM.Class);
	}

	//콜리전 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IBCharacter"));

	AIControllerClass = AIB_E_GREATERSPIDER_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AIB_E_GreaterSpider::BeginPlay()
{
	Super::BeginPlay();
	
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

