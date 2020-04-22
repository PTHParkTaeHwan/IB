// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Blueprint/UserWidget.h"
#include "IBCharacterSelecterWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIBCharacterSelecterWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);

	virtual void NativeConstruct() override;




protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 MaxIndex;

	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;	
	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent2;
	

	UPROPERTY()
	class UButton* PrevButton;

	UPROPERTY()
	class UButton* NextButton;

	UPROPERTY()
	class UEditableTextBox* TextBox;

	UPROPERTY()
	class UButton* ConfirmButton;

public:
	UPROPERTY(EditAnywhere)
	AActor* CameraOne;



private:
	UFUNCTION()
	void OnPrevClicked();

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnConfirmClicked();


private:
	bool bLeftFirstClicked;
	bool bRightFirstClicked;

	bool bLevelOpen;
	float fLevelOpenBufferTime;


};
