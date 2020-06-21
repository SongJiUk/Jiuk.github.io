// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMyCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected :

	/* 블루프린트 그래프에서 실행 가능한 가장 일반적인 specifier.
C++의 구현 내용을 블루프린트에서 재정의할 수 없으며, 호출만 가능하기에 대체적으로 setter 성격의 함수에 많이 사용된다. */
	//블루프린트에서 사용할수 있도록 지정
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);

	virtual void NativeConstruct() override;

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 MaxIndex;

	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	UPROPERTY()
	class UButton* PrevButton;

	UPROPERTY()
	class UButton* NextButton;

	UPROPERTY()
	class UEditableTextBox* TextBox;

	UPROPERTY()
	class UButton* ConfirmButton;

private :
	UFUNCTION()
	void OnPrevClicked();

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnConfirmClicked();
};
