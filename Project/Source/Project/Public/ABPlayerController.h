// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
/*�÷��̾� ��Ʈ�ѷ����� �÷��̾��� �Է°� ��µǴ� ȭ���� å������.
�÷��� ��ư�� ���� �������� �׽�Ʈ�� ���� �Ź� �𸮾� ������ ����Ʈ�� Ŭ���� ��Ŀ���� ��ƾ� �Է½�ȣ�� ��μ� ���ӿ� ���޵Ǵ� 
�������� �ִ�. �÷��̾� ��Ʈ�ѷ����� UI�� �����ϰ� ���ӿ��Ը� �Է��� �����ϵ��� ����� ������ ������ ���ϰ� ������ 
�׽�Ʈ�� �� �ִ�.
*/
#include "Project.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public :
	AABPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class UABHUDWidget* GetHUDWidget() const;
	void NPCKill(class AMyCharacter* KilledNPC) const;
	void AddGameScore() const;
	void ChangeInputMode(bool bGameMode = true);
	void ShowResultUI();

protected :
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABHUDWidget> HUDWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UMyGamePlayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UMyGameplayResultWidget> ResultWidgetClass;

private :
	UPROPERTY()
	class UABHUDWidget* HUDWidget;

	UPROPERTY()
	class AMyPlayerState* MyPlayerState;

	UPROPERTY()
	class UMyGameplayResultWidget* ResultWidget;

	void OnGamePause();

	UPROPERTY()
	class UMyGamePlayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
