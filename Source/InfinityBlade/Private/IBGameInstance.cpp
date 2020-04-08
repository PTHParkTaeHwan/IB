// Fill out your copyright notice in the Description page of Project Settings.

#include "IBGameInstance.h"

UIBGameInstance::UIBGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Book/GamaData/IBCharacterData.IBCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_IBCHARACTER(*CharacterDataPath);
	ABCHECK(DT_IBCHARACTER.Succeeded());
	IBCharacterTable = DT_IBCHARACTER.Object;
	ABCHECK(IBCharacterTable->RowMap.Num() > 0);

	CharacterDataPath=TEXT("/Game/Book/GamaData/IB_E_GSData.IB_E_GSData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_IB_E_GS(*CharacterDataPath);
	IB_E_GSTable = DT_IB_E_GS.Object;

}

FIBCharacterData * UIBGameInstance::GetIBCharacterData(int32 Level)
{
	return IBCharacterTable->FindRow<FIBCharacterData>(*FString::FromInt(Level), TEXT(""));
}

FIB_E_GSData * UIBGameInstance::GetIB_E_GSData(int32 Level)
{
	return IB_E_GSTable->FindRow<FIB_E_GSData>(*FString::FromInt(Level), TEXT(""));
}




