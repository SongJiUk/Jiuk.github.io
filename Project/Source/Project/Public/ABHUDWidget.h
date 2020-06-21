// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
	//전달받을 캐릭터 스텟과 스테이트정보
public :
	void BindCharacterStat(class UMyCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AMyPlayerState* PlayerState);

protected :
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private :
	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AMyPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;
	
	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentScore;

	UPROPERTY()
	class UTextBlock* HighScore;
};
