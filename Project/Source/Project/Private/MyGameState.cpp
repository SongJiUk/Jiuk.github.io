// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

AMyGameState::AMyGameState()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 AMyGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AMyGameState::AddGameScore()
{
	TotalGameScore++;
}

void AMyGameState::SetGameCleared()
{
	bGameCleared = true;
}

bool AMyGameState::IsGameCleared() const
{
	return bGameCleared;
}