// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/Character.h"
#include "IB_E_GreaterSpider.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
