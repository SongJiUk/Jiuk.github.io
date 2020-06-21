// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public :
	AABGameMode();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void AddScore(class AABPlayerController* ScoredPlayer);
	int GetScore() const;

private :
	UPROPERTY()
	class AMyGameState* MyGameState;

	UPROPERTY()
	int32 ScoreToClear;
};
