// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API AIBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AIBPlayerController();

	class UIBHUDWidget* GetHUDWidget() const;
	void NPCKill(class AIBCharacter* KilledNPC) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UIBHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
	class UIBHUDWidget* HUDWidget;

	UPROPERTY()
	class AIBPlayerState* IBPlayerState;
	
};
