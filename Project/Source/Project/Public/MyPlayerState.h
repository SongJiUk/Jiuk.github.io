// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/*해당 UI에는 플레이어의 데이터와 캐릭터의 HP정보가 함께 표시된다. 따라서 플레이어의 캐릭터 스탯컴포넌트 정보를 모두
해당 HUD에 연동해야됨.
새로운 델리게이트 정의하고 플레이어 데이터가 변경될때 HUD에 신호를 보내 HUD가 관련 UI위젯을 업데이트하도록 구현
*/
DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class PROJECT_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	
public :
	AMyPlayerState();


	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	int32 GetGameHighScore() const;
	int32 GetCharacterIndex() const;
	FString SaveSlotName;

	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	void AddGameScore();

	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected :
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;

	UPROPERTY(Transient)
	int32 CharacterIndex;

private :
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FMyCharacterData* CurrentStatData;

};
