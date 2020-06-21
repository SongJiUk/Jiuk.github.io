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
	�����½�ũ�� ���� �ִϸ��̼��� ���������� ����ؾ� �ϴ� ���� �½�ũ�̹Ƿ� ExecuteTask�� ������� InProgress��
	�ϴ� ��ȯ�ϰ� ������ ������ �� �½�ũ�� �����ٰ� �˷���� �Ѵ�.	�̸� �˷��ִ� �Լ��� FinishLatentTask��.
	�½�ũ���� �� �Լ��� ���߿� ȣ�������� ������ �����̺�� Ʈ�� �ý����� ���� �½�ũ�� ��� �ӹ��� �ȴ�.

	���Ŀ� FinishLatentTask�� ȣ���� �� �ֵ��� ����� Tick����� Ȱ��ȭ�ϰ� Tick���� ������ �ľ��� �� �½�ũ ���� ��� ���������.
	*/
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected :
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private :
	bool IsAttacking = false;
};
