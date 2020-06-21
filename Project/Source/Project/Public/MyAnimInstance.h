// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
/**
 * 
 */
UCLASS()
class PROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public :
	UMyAnimInstance();
	//���� �����Ϳ� �����ϱ����� �ִ� �ν��Ͻ��� Tick���� ���� �ӵ� ������ �������� �̸� CurrentPawnSpeed�� ������Ʈ
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//���� �ִϸ��̼�
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private :
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

private :

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	//���ǵ带 �Ǵ��ؼ� ���� ������ �Ǵ�.
	float CurrentPawnSpeed;
	//
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
