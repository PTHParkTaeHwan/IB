// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Components/ActorComponent.h"
#include "IB_E_GS_StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(E_FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(E_FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFINITYBLADE_API UIB_E_GS_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIB_E_GS_StatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);

	float GetAttack();
	float GetHPRatio();
	int32 GetDropExp() const;

	E_FOnHPIsZeroDelegate E_OnHPIsZero;
	E_FOnHPChangedDelegate E_OnHPChanged;

private:
	struct FIB_E_GSData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;


};
