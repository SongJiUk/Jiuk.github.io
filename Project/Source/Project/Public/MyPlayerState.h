// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/*�ش� UI���� �÷��̾��� �����Ϳ� ĳ������ HP������ �Բ� ǥ�õȴ�. ���� �÷��̾��� ĳ���� ����������Ʈ ������ ���
�ش� HUD�� �����ؾߵ�.
���ο� ��������Ʈ �����ϰ� �÷��̾� �����Ͱ� ����ɶ� HUD�� ��ȣ�� ���� HUD�� ���� UI������ ������Ʈ�ϵ��� ����
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
