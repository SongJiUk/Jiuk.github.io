// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MyGameInstance.h"
#include "MySaveGame.h"

AMyPlayerState::AMyPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	SaveSlotName = TEXT("Player1");
	CharacterIndex = 0;
}

int32 AMyPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AMyPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 AMyPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 AMyPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

void AMyPlayerState::InitPlayerData()
{
	auto MySaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == MySaveGame)
	{
		MySaveGame = GetMutableDefault<UMySaveGame>();
	}

	SetPlayerName(MySaveGame->PlayerName);
	SetCharacterLevel(MySaveGame->Level);
	GameScore = 0;
	GameHighScore = MySaveGame->HighScore;
	Exp = MySaveGame->Exp;
	CharacterIndex = MySaveGame->CharacterIndex;
	SavePlayerData();
}

void AMyPlayerState::SavePlayerData()
{
	/*언리얼 오브젝트를 생성할땐 NewObject명령을 사용하며, 생성된 오브젝트를 더 이상 사용하지 않으면 자동으로 소멸시킨다.*/
	UMySaveGame* NewPlayerData = NewObject<UMySaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}

float AMyPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	ABLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool AMyPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
	return DidLevelUp;
}

void AMyPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void AMyPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	ABCHECK(nullptr != MyGameInstance);

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}