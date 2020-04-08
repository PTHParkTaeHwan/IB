// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "IBGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FIBCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FIBCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30), WalkSpeed(10.0f), ShieldEnergy(100.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ShieldEnergy;
};

USTRUCT(BlueprintType)
struct FIB_E_GSData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FIB_E_GSData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;
};

UCLASS()
class INFINITYBLADE_API UIBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UIBGameInstance();

	FIBCharacterData* GetIBCharacterData(int32 Level);
	FIB_E_GSData* GetIB_E_GSData(int32 Level);

	FStreamableManager StreamableManager;
	FStreamableManager E_GS_StreamableManager;
	
private:
	UPROPERTY()
	class UDataTable* IBCharacterTable;	

	UPROPERTY()
	class UDataTable* IB_E_GSTable;
};
