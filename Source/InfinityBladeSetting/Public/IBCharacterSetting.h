// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IBCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=InfinityBlade)
class INFINITYBLADESETTING_API UIBCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UIBCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
	
	
	
};
