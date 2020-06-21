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
	//폰의 데이터와 연동하기위해 애님 인스턴스의 Tick에서 폰의 속도 정보를 가져온후 이를 CurrentPawnSpeed에 업데이트
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//공격 애니메이션
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
	//스피드를 판단해서 뛸지 걸을지 판단.
	float CurrentPawnSpeed;
	//
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
