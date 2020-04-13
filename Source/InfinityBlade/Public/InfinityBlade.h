// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

enum class EnemyMode
{
	/*
	1. 패트롤 모드였다가
	2. 플레이어를 발견하면 함성을 한번 지르고
	3. 플레이어에게 달려간다
	4. 플레이어를 공격할 수 있는 거리가 되면 전투모드로 넘어간다
	5. 플레이어를 중심으로 왼쪽, 오른쪽으로 왔다가닸다 하다가 공격한다.
	6. 공격이 시작되면 콤보로 공격한다.
	7. 플레이어의 공격에 피격당하면 피격모션을 보여준 후 다시 전투모드로 넘어간다.
	*/
	PATROL,
	TARGETON,
	TENTIONON,
	ATTACK,
	HIT
};

//플레이어 공격 모션
enum class WeaponType
{
	LONGSWORD,
	MIDDLESWORD
};


DECLARE_LOG_CATEGORY_EXTERN(InfinityBlade, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__)+TEXT(")"))
#define ABLOG_S(Verbosty) UE_LOG(InfinityBlade, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(InfinityBlade, Verbosity, TEXT("%s%s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}