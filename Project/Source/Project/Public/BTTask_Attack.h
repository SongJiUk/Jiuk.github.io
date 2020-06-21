// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public :
	UBTTask_Attack();

	/*
	공격태스크는 공격 애니메이션이 끝날때까지 대기해야 하는 지연 태스크이므로 ExecuteTask의 결과값을 InProgress로
	일단 반환하고 공격이 끝날을 때 태스크가 끝났다고 알려줘야 한다.	이를 알려주는 함수가 FinishLatentTask다.
	태스크에서 이 함수를 나중에 호출해주지 않으면 비헤이비어 트리 시스템은 현재 태스크에 계속 머물게 된다.

	차후에 FinishLatentTask를 호출할 수 있도록 노드의 Tick기능을 활성화하고 Tick에서 조건을 파악한 후 태스크 종료 명령 내려줘야함.
	*/
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected :
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private :
	bool IsAttacking = false;
};
