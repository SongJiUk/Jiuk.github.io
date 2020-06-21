// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
//플레이어의 공격이 종료되면 공격 태스크에서 해당 알림을 받을 수 있도록 선언 공격이 종료되면 이를 호출하는 로직 캐릭터에 구현
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	//해당 열거형 타입의 변수를 선언하고, 스테이트 머신 제작을위한 기본 로직과 함수 선언.
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	int32 GetExp() const;
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		HERO,
		NPC
	};
	
	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::HERO;
	FVector DirectionToMove = FVector::ZeroVector;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//캐릭터에 무기를 장착시키는 SetWeapon 멤버함수 선언
	//현재 무기가 없으면 hand_rSocket에 무기를 장착시키고 무기 액터의 소유자를 캐릭터로 변경시키는 로직 삽입
	bool CanSetWeapon();
	void SetWeapon(class AMyWeapon* NewWeapon);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AMyWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UMyCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	
	//액터에 UI위젯을 부착할 수 있도록 UWidgetComponent라는 클래스 제공
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	//NPC도 사용하기위해 public으로 변경
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

private:
	//위아래 좌우이동 키입력
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void OnAssetLoadCompleted();
	

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta=(AllowPrivateAccess=true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta=(AllowPrivateAccess=true))
	bool bIsPlayer;

	UPROPERTY()
	class AABAIController* ABAIController;

	UPROPERTY()
	class AABPlayerController* ABPlayerController;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	//애님 인스턴스를 자주 사용할 예정이라 생성
	UPROPERTY()
	class UMyAnimInstance* MyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State, Meta=(AllowPrivateAccess=true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = { };
};
