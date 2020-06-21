// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
/*
내비메시 영역을 생성하면, 이를 활용해 ABAIController에 빙의한 폰에게 목적지를 알려줘 폰이 목적지까지 스스로
움직이도록 명령 추가. 그리고 AI컨트롤러에 타이머를 설치해 3초마다 폰에게 목적지로 이동하는 명령 내림

언리얼엔진의 내비게이션 시스템은 이동 가능한 목적지를 랜덤으로 가져오는
GetRandomPointInNavigableRadius 함수와 목적지로 폰을 이동시키는 SimpleMoveToLocation함수를 제공
*/
public :
	AABAIController();
	virtual void OnPossess(APawn* InPawn) override;

	//NPC생성됐을때의 위치값, 앞으로 순찰할 위치정보
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	//플레이어 캐릭터에게 지정/ 아니면 nullptr 지정
	static const FName TargetKey;

	//스테이트에 맞게 비헤이비어 트리 로직을 수동으로 구동하고 중지할 수 있게 AI컨트롤러의 구조를 변경
	void RunAI();
	void StopAI();

private :
	//블랙보드에셋, 비헤이비어트리 선언

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
