// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "Components/ActorComponent.h"
#include "MyCharacterStatComponent.generated.h"

//ĳ���Ϳ� �������� ������ �ʵ��� ���� ������Ʈ�� ��������Ʈ�� �����ϰ� ĳ���Ϳ��� �̸� ���ε�
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
//��ȭ�������� ������ �ʰ� �ϱ� ���� ��������Ʈ ����, ������Ʈ�� HP���� ���� ������ UI������ ���� �ڵ����� ����ǵ��� ����
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//���ȿ� ������ �Ͼ ���� ���õ����͸� ó���� ���� Tick���� �ʿ� x
	//������ PostInitializeComponents�� �����ϴ� ������Ʈ�� �Լ�. ������ Post--�Լ��� ȣ��Ǳ� �ٷ����� ȣ��
	//�� �Լ��� �̿��Ͽ� �ʱ�ȭ ������ ����. ȣ��Ƿ��� ������ �Լ����� bWantsInitializeComponent���� true�� ����������Ѵ�.
	virtual void InitializeComponent() override;

public :
	//������ SetNewLevel�� ���ؼ��� ������ �� �ֵ��� ����.
	void SetNewLevel(int32 NewLevel);
	
	//SetDamage�Լ��� �����ϰ� ĳ������ TakeDamage �Լ����� �̸� ȣ��
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	float GetAttack() const;
	float GetHPRatio() const;
	int32 GetDropExp() const;

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

private :
	//������������ HP�������� ��� ������ ���� ������Ʈ���� �����ϵ��� ���� �߰� ����
	struct FMyCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	//������ �����Ҷ����� ����Ǵ� ���̹Ƿ� �� ���� �����ϴ°��� �ǹ̰� ����. Transient Ű���带 �߰��Ͽ� ����ȭ���� ���ܽ�Ű�°��� ����.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
