// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
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
	void NPCKill(AActor* KilledNPC /*AIBCharacter* KilledNPC*/) const;
	void ChangeInputMode(bool bGameMode = true);


protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UIBHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UIBGameplayWidget> MenuWidgetClass;

private:
	void OnGamePause();

	UPROPERTY()
	class UIBHUDWidget* HUDWidget;

	UPROPERTY()
	class AIBPlayerState* IBPlayerState;

	UPROPERTY()
	class UIBGameplayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
	
};
