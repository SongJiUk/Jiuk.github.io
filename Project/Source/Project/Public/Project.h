// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

//코드 카테고리를 Project로 고정하고 로그르르 남길때 추가로 발생한 함수 이름과 코드 라인을 함께 출력
//ABLOG_S : 코드가 들어있는 파일 이름과 함수, 그리고 라인 정보를 추가해 Project카테고리로 로그를 남김. 실행시점파악할때 유용
//ABLOG : ABLOG_S 정보에 형식 문자열로 추가 정보를 지정해 로그 남김

UENUM(BlueprintType)
//캐릭터의 스테이트를 구분하도록 새로운 열거형 정의.
enum class ECharacterState : uint8
{
	PREINIT, //캐릭터 생성 전의 스테이트, 기본 캐릭터 애셋이 설정돼 있지만 캐릭터와 UI를 숨겨둔다. 대미지 입지 않음.
	LOADING, /*선택한 캐릭터 애셋을 로딩하는 스테이트, 이때는 게임이 시작된 시점 현재 조정하는 컨트롤러가 AI인지 플레이어인지 구별 가능
			  플레이어 컨트롤러인경우 애셋 로딩이 완료될 때까지 캐릭터를 조종하지 못하돌고 입력 비활성화.*/
	READY,  /*캐릭터 애셋 로딩이 완료된 스테이트, 숨겨둔 캐릭터와 UI를 보여주며, 이때부터는 공격을 받으면 대미지를 입는다.
			플레이어 컨트롤러는 비로소 캐릭터를 조종할 수 있으며, AI컨트롤러는 비헤이비어 트리 로직을 구동해 캐릭터 동작시킨다.*/
	DEAD /*캐릭터가 HP를 소진해 사망할때 발생하는 스테이트. 죽는 애니메이션을 재생하고 UI를 끄는 한편, 충돌 기능을 없애고
		대미지를 입지않도록 설정한다. 컨트롤러가 플레이어인경우 입력 비활성화, AI인경우 비헤이비어 트리 로직 중지 
		일정시간이 지난 후에는 플레이어의 경우 레벨을 재시작하고, AI경우 레벨에서 퇴장한다.*/
};

DECLARE_LOG_CATEGORY_EXTERN(Project, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(Project, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(Project, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define ABCHECK(Expr, ...) {if(!(Expr)) {ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

