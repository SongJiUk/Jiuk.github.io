// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

//�ڵ� ī�װ��� Project�� �����ϰ� �α׸��� ���涧 �߰��� �߻��� �Լ� �̸��� �ڵ� ������ �Բ� ���
//ABLOG_S : �ڵ尡 ����ִ� ���� �̸��� �Լ�, �׸��� ���� ������ �߰��� Projectī�װ��� �α׸� ����. ��������ľ��Ҷ� ����
//ABLOG : ABLOG_S ������ ���� ���ڿ��� �߰� ������ ������ �α� ����

UENUM(BlueprintType)
//ĳ������ ������Ʈ�� �����ϵ��� ���ο� ������ ����.
enum class ECharacterState : uint8
{
	PREINIT, //ĳ���� ���� ���� ������Ʈ, �⺻ ĳ���� �ּ��� ������ ������ ĳ���Ϳ� UI�� ���ܵд�. ����� ���� ����.
	LOADING, /*������ ĳ���� �ּ��� �ε��ϴ� ������Ʈ, �̶��� ������ ���۵� ���� ���� �����ϴ� ��Ʈ�ѷ��� AI���� �÷��̾����� ���� ����
			  �÷��̾� ��Ʈ�ѷ��ΰ�� �ּ� �ε��� �Ϸ�� ������ ĳ���͸� �������� ���ϵ��� �Է� ��Ȱ��ȭ.*/
	READY,  /*ĳ���� �ּ� �ε��� �Ϸ�� ������Ʈ, ���ܵ� ĳ���Ϳ� UI�� �����ָ�, �̶����ʹ� ������ ������ ������� �Դ´�.
			�÷��̾� ��Ʈ�ѷ��� ��μ� ĳ���͸� ������ �� ������, AI��Ʈ�ѷ��� �����̺�� Ʈ�� ������ ������ ĳ���� ���۽�Ų��.*/
	DEAD /*ĳ���Ͱ� HP�� ������ ����Ҷ� �߻��ϴ� ������Ʈ. �״� �ִϸ��̼��� ����ϰ� UI�� ���� ����, �浹 ����� ���ְ�
		������� �����ʵ��� �����Ѵ�. ��Ʈ�ѷ��� �÷��̾��ΰ�� �Է� ��Ȱ��ȭ, AI�ΰ�� �����̺�� Ʈ�� ���� ���� 
		�����ð��� ���� �Ŀ��� �÷��̾��� ��� ������ ������ϰ�, AI��� �������� �����Ѵ�.*/
};

DECLARE_LOG_CATEGORY_EXTERN(Project, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(Project, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(Project, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define ABCHECK(Expr, ...) {if(!(Expr)) {ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

