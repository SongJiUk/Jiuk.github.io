// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "MyWeapon.h"
#include "MyCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "MyCharacterWidget.h"
#include "ABAIController.h"
#include "MyCharacterSetting.h"
#include "MyGameInstance.h"
#include "ABPlayerController.h"
#include "MyPlayerState.h"
#include "ABHUDWidget.h"
#include "ABGameMode.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	//private�� ����� ������Ʈ�� �����͸� ��ӹ��� Ŭ�������� ������ �� �ֵ��� GetCapusuleComponent, GetMesh, GetCharacterMovement ����
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//ī�޶� ������ ����
	SpringArm->TargetArmLength = 400.0f;
	//ī�޶� ����
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	//�ִϸ��̼� �������Ʈ ĳ���� ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASE(TEXT("/Game/Scanned3DPeoplePack/RP_Character/rp_manuel_rigged_001_ue4/rp_manuel_rigged_001_ue4.rp_manuel_rigged_001_ue4"));
	if (SK_BASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BASE.Object);
	}

	//�ִϸ��̼� �������Ʈ ���� �� ���
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Game/Book/Animations/AnimBluePrint.AnimBluePrint_C"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}

	SetControlMode(EControlMode::HERO);

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}
	}
	Weapon->SetupAttachment(GetMesh(), WeaponSocket);

	//HPBar���� �Ӹ����� ������
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	//�׻� �÷��̾ ���� ������
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		//ũ��� �۾��������� ������ �Ͱ� ������ 150,50 ���� ����
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	//ABCharacter�� �̸� ����ϵ��� AIController�� Ŭ���� �Ӽ��� ABAIController�� Ŭ������ ���ϰ�
	//AI�� ���� �ɼ��� PlacedInWorldorSpawned�� ���� �׷��� ������ ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� ABCharacter����
	//ABAIController ���Ͱ� �����ǰ�, �÷��̾ �����ϴ� ĳ���͸� ������ ��� ĳ���ʹ� �� ABAIController�� ���踦 �޴´�.
	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//�÷��̾ ĳ���͸� �����ϴ� ��쿡�� �ӽ÷� 4���ε����� ĳ���� ���������, AI�� ������ �������� �ϳ� ��� ���
	AssetIndex = 4;
	AttackRange = 80.0f;


	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);

	DeadTimer = 5.0f;
}

float AMyCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

float AMyCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;
	return AttackDamage * AttackModifier;
}

int32 AMyCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/*FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		//�� ������ RootComponent�� ������ ������Ʈ��, ���������� ���� ���Ͽ� �����Ѵ�.
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	*/

	
	//ĳ���� ������Ʈ�� UI������ ����
	/*
	auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	
	if (!IsPlayerControlled())
	{
		auto DefaultSetting = GetDefault<UMyCharacterSetting>();
		int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

		auto MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if (nullptr != MyGameInstance)
		{
			FStreamable���� �񵿱� ������� �ּ��� �ε��ϴ� ����� AsyncLoad�̴�.
			�ش� �Լ��� FStreamableDelegate ������ ��������Ʈ�� �Ѱ��� ���, �ּ��� �ε��� �Ϸ��ϸ�
			�ش� ��������Ʈ�� ����� �Լ��� ȣ�����ش�.
			��������Ʈ���� �����ϴ� CreateUObject����� ����Ͽ� �Ｎ���� ��������Ʈ�� ���������ν� �Լ��� ������Ų ��
			�Ѱ��ִ� ����� ��
			AssetStreamingHandle = MyGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMyCharacter::OnAssetLoadCompleted));
		}
	} */


	/*ĳ���ʹ� �ڽ��� ���ΰ����� NPC���� �Ǻ��� �� �־�� �ϴµ�, �̸� Ȯ���� �ľ��� �� �ִ� ���� �� �ϳ��� BeginPlay�̴�
	���� ������ �÷��̾ ���� AI��Ʈ�ѷ��� �ڵ����� �����ǹǷ� PossessedBy�Լ��� �ι� ȣ��Ǳ� �����̴�.
	ĳ���ʹ� PREINIT ������Ʈ���� �����Ѵ�. �׷��ٰ� ������ ���۵� BeginPlay�Լ��� ȣ���ϸ� �÷��̾�����, NPC���� �ľ���
	�����ϴ� ���� IsPlayer�� �� ����� �����Ѵ�. �̾ ���õ� ĳ���� �ּ��� �ε��� �����ϰ� LOADING���� �����Ѵ�.*/
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		ABPlayerController = Cast<AABPlayerController>(GetController());
		ABCHECK(nullptr != ABPlayerController);
	}
	else
	{
		ABAIController = Cast<AABAIController>(GetController());
		ABCHECK(nullptr != ABAIController);
	}

	//INI���� �ҷ���
	auto DefaultSetting = GetDefault<UMyCharacterSetting>();

	if (bIsPlayer)
	{
		auto MyPlayerState = Cast<AMyPlayerState>(PlayerState);
		ABCHECK(nullptr != MyPlayerState);
		AssetIndex = MyPlayerState->GetCharacterIndex();
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	ABCHECK(nullptr != MyGameInstance);
	AssetStreamingHandle = MyGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMyCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);
}

void AMyCharacter::SetCharacterState(ECharacterState NewState)
{
	ABCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING :
	{

		//�÷��̾ �����Ҷ� �߻��ϴ� ĳ������ PossessedBy�Լ��� �����ϰ� ��� ĳ������ READY������Ʈ���� ����
		if (bIsPlayer)
		{
			DisableInput(ABPlayerController);

			ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);
			
			auto MyPlayerState = Cast<AMyPlayerState>(PlayerState);
			ABCHECK(nullptr != MyPlayerState);
			CharacterStat->SetNewLevel(MyPlayerState->GetCharacterLevel());

		}
		else
		{
			auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
			ABCHECK(nullptr != ABGameMode);
			int32 TargetLevel = FMath::CeilToInt(((float)ABGameMode->GetScore() * 0.8f));
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			ABLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			CharacterStat->SetNewLevel(FinalLevel);
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}
	case ECharacterState::READY :
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			SetCharacterState(ECharacterState::DEAD);
			});

		auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		ABCHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::HERO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(ABPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			ABAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD :
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		MyAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			DisableInput(ABPlayerController);
		}
		else
		{
			ABAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			
			if (bIsPlayer)
			{
				ABPlayerController->ShowResultUI();
			}
			else
			{
				Destroy();
			}
			}), DeadTimer, false);

		break;
	}
	}
}

ECharacterState AMyCharacter::GetCharacterState() const
{
	return CurrentState;
}

//���⸦ �����Ҽ� �ִ��� Ȯ��
bool AMyCharacter::CanSetWeapon()
{
	return true;
}

void AMyCharacter::SetWeapon(AMyWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon);

	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	//Ȯ��
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//������ ����(�ڱ��ڽ�)
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AMyCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;
	switch(CurrentControlMode)
	{
	case EControlMode::HERO :
		SpringArm->TargetArmLength = 2000.f;
		SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::NPC :
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DirectionToMove.Size() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���Ʒ�, �¿� ����
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	//���ݼ���
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

}

void AMyCharacter::UpDown(float NewAxisValue)
{
		
	DirectionToMove.X = NewAxisValue;
}

void AMyCharacter::LeftRight(float NewAxisValue)
{

	DirectionToMove.Y = NewAxisValue;

}

void AMyCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	//ó�� �����Ҷ� READY���·� ����
	SetCharacterState(ECharacterState::READY);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != MyAnim);

	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {

		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});
	//��������Ʈ �߰�
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

	//ĳ���ͽ��ȿ��� ������ ��������Ʈ�� ���ٽ����� ���ε�
	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

//�������� �����°�
float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	
	//SetDamage�Լ�ȣ��
	CharacterStat->SetDamage(FinalDamage);
	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto ABPlayerController = Cast<AABPlayerController>(EventInstigator);
			ABCHECK(nullptr != ABPlayerController, 0.0f);
			ABPlayerController->NPCKill(this);
		}
	}
	return FinalDamage;
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}
//ĳ���Ϳ��� ��Ÿ�ָ� ����Ͽ� ���� �ִϸ��̼��� ����϶�� ����� ����
void AMyCharacter::Attack()
{
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		MyAnim->PlayAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}
//��������Ʈ
void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	//������ ������ �˷���
	OnAttackEnd.Broadcast();
}

void AMyCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMyCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMyCharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();

	FHitResult HitResult;
	//�˻��Ͽ� �ڽ��� Ž���� ���� �ʵ��� ����
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			//50��
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}