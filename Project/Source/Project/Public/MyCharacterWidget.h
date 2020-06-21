// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMyCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public :
	void BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat);

protected :
	//UI시스템이 준비되면 Native--함수가 호출된다. 여기에 로직구현
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private :
	//약포인터를 사용해 컴포넌트 참조
	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
