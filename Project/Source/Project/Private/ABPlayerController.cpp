// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "ABHUDWidget.h"
#include "MyPlayerState.h"
#include "MyCharacter.h"
#include "MyCharacterStatComponent.h"
#include "MyGamePlayWidget.h"
#include "MyGameplayResultWidget.h"
#include "MyGameState.h"

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UMyGamePlayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UMyGameplayResultWidget> UI_RESULT_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

UABHUDWidget* AABPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(true);

	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	ABCHECK(nullptr != HUDWidget)
	HUDWidget->AddToViewport(1);

	ResultWidget = CreateWidget<UMyGameplayResultWidget>(this, ResultWidgetClass);
	ABCHECK(nullptr != ResultWidget);

	MyPlayerState = Cast<AMyPlayerState>(PlayerState);
	ABCHECK(nullptr != MyPlayerState);
	HUDWidget->BindPlayerState(MyPlayerState);
	MyPlayerState->OnPlayerStateChanged.Broadcast();

}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AABPlayerController::OnGamePause);
}

void AABPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UMyGamePlayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

void AABPlayerController::NPCKill(AMyCharacter* KilledNPC) const
{
	(MyPlayerState->AddExp(KilledNPC->GetExp()));
	
}

void AABPlayerController::AddGameScore() const
{
	MyPlayerState->AddGameScore();
}

void AABPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void AABPlayerController::ShowResultUI()
{
	auto MyGameState = Cast<AMyGameState>(UGameplayStatics::GetGameState(this));
	ABCHECK(nullptr != MyGameState);
	ResultWidget->BindGameState(MyGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}