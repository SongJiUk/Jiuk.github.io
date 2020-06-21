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

//�ڽ��� ���� ������ ��尡 Ȱ��ȭ�� ��� �ֱ������� TickNode �Լ��� ȣ���Ѵ�.
//ȣ���ϴ� �ֱ�� ���� ��� ���ο� ������ Interval�Ӽ� ������ ������ �� �ִ�.
public :
	UBTService_Detect();

protected :
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
