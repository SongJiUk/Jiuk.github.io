// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "MyCharacter.h"
#include "ABPlayerController.h"
#include "MyPlayerState.h"
#include "MyGameState.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
	ScoreToClear = 2;
}

void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyGameState = Cast<AMyGameState>(GameState);
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	/*플레이어 컨트롤러가 초기화될 때 함께 해당 클래스의 인스턴스를 생성하고 그 포인터 값을 플레이어 컨트롤러의
	PlayerState 속성에 저장한다. 플레이어 컨트롤러의 구성을 완료하는 시점은 게임 모드의 PostLogin함수이므로 이때
	함께 MyPlayerState의 초기화도 완료해주는것이 좋다.
	새로운 플레이어 스테이트의 초기화
	*/
	auto MyPlayerState = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != MyPlayerState);
	MyPlayerState->InitPlayerData();
}

void AABGameMode::AddScore(AABPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
		if ((nullptr != ABPlayerController) && ScoredPlayer == ABPlayerController)
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}
	MyGameState->AddGameScore();

	if (GetScore() >= ScoreToClear)
	{
		MyGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
			if (nullptr != ABPlayerController)
			{
				ABPlayerController->ShowResultUI();
			}
		}
	}
}

int AABGameMode::GetScore() const
{
	return MyGameState->GetTotalGameScore();
}