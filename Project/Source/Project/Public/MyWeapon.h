// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class PROJECT_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

	float GetAttackRange() const;
	// ���ݽ� ���Ⱑ �ִ°�� ���� ���ݷ��� ������Ű�� ��� ���� ���⿡ ���� �������ݷ� ȿ��ġ �Ӽ� ����
	float GetAttackDamage() const;
	float GetAttackModifier() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackDamageMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackDamageMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackModifierMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackModifierMax;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	float AttackDamage;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	float AttackModifier;

public:	
	// Called every frame
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

};
