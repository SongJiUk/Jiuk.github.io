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

	/*�÷��̾� ��Ʈ�ѷ��� �ʱ�ȭ�� �� �Բ� �ش� Ŭ������ �ν��Ͻ��� �����ϰ� �� ������ ���� �÷��̾� ��Ʈ�ѷ���
	PlayerState �Ӽ��� �����Ѵ�. �÷��̾� ��Ʈ�ѷ��� ������ �Ϸ��ϴ� ������ ���� ����� PostLogin�Լ��̹Ƿ� �̶�
	�Բ� MyPlayerState�� �ʱ�ȭ�� �Ϸ����ִ°��� ����.
	���ο� �÷��̾� ������Ʈ�� �ʱ�ȭ
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