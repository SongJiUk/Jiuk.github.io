// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "MyGamePlayWidget.h"
#include "MyGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMyGameplayResultWidget : public UMyGamePlayWidget
{
	GENERATED_BODY()
	
public :
	void BindGameState(class AMyGameState* GameState);
	
protected :
	virtual void NativeConstruct() override;

private :
	TWeakObjectPtr<class AMyGameState> CurrentGameState;
};
