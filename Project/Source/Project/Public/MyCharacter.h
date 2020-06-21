// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
//�÷��̾��� ������ ����Ǹ� ���� �½�ũ���� �ش� �˸��� ���� �� �ֵ��� ���� ������ ����Ǹ� �̸� ȣ���ϴ� ���� ĳ���Ϳ� ����
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	//�ش� ������ Ÿ���� ������ �����ϰ�, ������Ʈ �ӽ� ���������� �⺻ ������ �Լ� ����.
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

	//ĳ���Ϳ� ���⸦ ������Ű�� SetWeapon ����Լ� ����
	//���� ���Ⱑ ������ hand_rSocket�� ���⸦ ������Ű�� ���� ������ �����ڸ� ĳ���ͷ� �����Ű�� ���� ����
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
	
	//���Ϳ� UI������ ������ �� �ֵ��� UWidgetComponent��� Ŭ���� ����
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	//NPC�� ����ϱ����� public���� ����
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

private:
	//���Ʒ� �¿��̵� Ű�Է�
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

	//�ִ� �ν��Ͻ��� ���� ����� �����̶� ����
	UPROPERTY()
	class UMyAnimInstance* MyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State, Meta=(AllowPrivateAccess=true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = { };
};
