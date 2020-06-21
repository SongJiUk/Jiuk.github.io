// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()

//자신이 속한 컴포짓 노드가 활성화될 경우 주기적으로 TickNode 함수를 호출한다.
//호출하는 주기는 서비스 노드 내부에 설정된 Interval속성 값으로 지정할 수 있다.
public :
	UBTService_Detect();

protected :
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
