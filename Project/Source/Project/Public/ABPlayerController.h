// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
/*플레이어 컨트롤러에서 플레이어의 입력과 출력되는 화면을 책임진다.
플레이 버튼을 눌러 콘텐츠를 테스트할 때는 매번 언리얼 에디터 뷰포트를 클릭해 포커스를 잡아야 입력신호가 비로소 게임에 전달되는 
불편함이 있다. 플레이어 컨트롤러에게 UI를 배제하고 게임에게만 입력을 전달하도록 명령을 내리면 앞으로 편리하게 게임을 
테스트할 수 있다.
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
