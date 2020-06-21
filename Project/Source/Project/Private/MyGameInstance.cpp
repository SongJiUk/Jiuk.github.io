// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Book/GameData/MyCharacterData.MyCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
	ABCHECK(DT_ABCHARACTER.Succeeded());
	MyCharacterTable = DT_ABCHARACTER.Object;
	ABCHECK(MyCharacterTable->GetRowMap().Num() > 0);
}

void UMyGameInstance::Init()
{
	Super::Init();

}

FMyCharacterData* UMyGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}