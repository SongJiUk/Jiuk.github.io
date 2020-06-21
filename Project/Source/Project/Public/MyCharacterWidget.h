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
	//UI�ý����� �غ�Ǹ� Native--�Լ��� ȣ��ȴ�. ���⿡ ��������
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private :
	//�������͸� ����� ������Ʈ ����
	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
