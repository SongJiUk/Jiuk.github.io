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
����޽� ������ �����ϸ�, �̸� Ȱ���� ABAIController�� ������ ������ �������� �˷��� ���� ���������� ������
�����̵��� ��� �߰�. �׸��� AI��Ʈ�ѷ��� Ÿ�̸Ӹ� ��ġ�� 3�ʸ��� ������ �������� �̵��ϴ� ��� ����

�𸮾����� ������̼� �ý����� �̵� ������ �������� �������� ��������
GetRandomPointInNavigableRadius �Լ��� �������� ���� �̵���Ű�� SimpleMoveToLocation�Լ��� ����
*/
public :
	AABAIController();
	virtual void OnPossess(APawn* InPawn) override;

	//NPC������������ ��ġ��, ������ ������ ��ġ����
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	//�÷��̾� ĳ���Ϳ��� ����/ �ƴϸ� nullptr ����
	static const FName TargetKey;

	//������Ʈ�� �°� �����̺�� Ʈ�� ������ �������� �����ϰ� ������ �� �ְ� AI��Ʈ�ѷ��� ������ ����
	void RunAI();
	void StopAI();

private :
	//�����忡��, �����̺��Ʈ�� ����

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
