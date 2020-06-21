// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "Components/ActorComponent.h"
#include "MyCharacterStatComponent.generated.h"

//캐릭터에 의존성을 가지지 않도록 액터 컴포넌트에 델리게이트를 선언하고 캐릭터에서 이를 바인딩
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
//상화의존성을 가지지 않게 하기 위해 델리게이트 선언, 컴포넌트의 HP값이 변할 때마다 UI위젯의 값이 자동으로 변경되도록 설계
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

	//스탯에 변경이 일어날 때만 관련데이터를 처리할 에정 Tick로직 필요 x
	//액터의 PostInitializeComponents에 대응하는 컴포넌트의 함수. 액터의 Post--함수가 호출되기 바로전에 호출
	//이 함수를 이용하여 초기화 로직을 구현. 호출되려면 생성자 함수에서 bWantsInitializeComponent값을 true로 설정해줘야한다.
	virtual void InitializeComponent() override;

public :
	//레벨을 SetNewLevel을 통해서만 변경할 수 있도록 설정.
	void SetNewLevel(int32 NewLevel);
	
	//SetDamage함수를 생성하고 캐릭터의 TakeDamage 함수에서 이를 호출
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	float GetAttack() const;
	float GetHPRatio() const;
	int32 GetDropExp() const;

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

private :
	//레벨정보에서 HP정보까지 모든 스탯을 스탯 컴포넌트에서 관리하도록 변수 추가 선언
	struct FMyCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	//게임을 시작할때마다 변경되는 값이므로 이 값을 보관하는것은 의미가 없다. Transient 키워드를 추가하여 직렬화에서 제외시키는것이 좋다.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
