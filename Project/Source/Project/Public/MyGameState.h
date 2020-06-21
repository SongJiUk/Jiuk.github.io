// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public :
	AMyGameState();

public :
	int32 GetTotalGameScore() const;
	void AddGameScore();
	void SetGameCleared();
	bool IsGameCleared() const;

private  :
	UPROPERTY(Transient)
	int32 TotalGameScore;
	
	UPROPERTY(Transient)
	bool bGameCleared;
};
