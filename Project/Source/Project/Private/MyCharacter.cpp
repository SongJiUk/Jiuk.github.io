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

	//private로 선언된 컴포넌트이 포인터를 상속받은 클래스들이 접근할 수 있도록 GetCapusuleComponent, GetMesh, GetCharacterMovement 제공
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//카메라 지지대 길이
	SpringArm->TargetArmLength = 400.0f;
	//카메라 각도
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	//애니메이션 블루프린트 캐릭터 경로
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASE(TEXT("/Game/Scanned3DPeoplePack/RP_Character/rp_manuel_rigged_001_ue4/rp_manuel_rigged_001_ue4.rp_manuel_rigged_001_ue4"));
	if (SK_BASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BASE.Object);
	}

	//애니메이션 블루프린트 설정 및 경로
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

	//HPBar설정 머리위에 오도록
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	//항상 플레이어를 향해 보도록
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		//크기는 작업공간에서 설정한 것과 동일한 150,50 으로 지정
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	//ABCharacter가 이를 사용하도록 AIController의 클래스 속성을 ABAIController의 클래스로 정하고
	//AI의 생성 옵션을 PlacedInWorldorSpawned로 설정 그러면 앞으로 레벨에 배치하거나 새롭게 생성되는 ABCharacter마다
	//ABAIController 액터가 생성되고, 플레이어가 조종하는 캐릭터를 제외한 모든 캐릭터는 이 ABAIController의 지배를 받는다.
	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//플레이어가 캐릭터를 조종하는 경우에는 임시로 4번인덱스의 캐릭터 에셋을사용, AI가 조정시 랜덤으로 하나 골라 사용
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
		//이 액터의 RootComponent를 제공된 컴포넌트에, 선택적으로 명명된 소켓에 연결한다.
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	*/

	
	//캐릭터 컴포넌트와 UI위젯을 연결
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
			FStreamable에서 비동기 방식으로 애셋을 로딩하는 명령은 AsyncLoad이다.
			해당 함수에 FStreamableDelegate 형식의 델리게이트를 넘겨줄 경우, 애셋이 로딩을 완료하면
			해당 델리게이트에 연결된 함수를 호출해준다.
			델리게이트에서 제공하는 CreateUObject명령을 사용하여 즉석에서 델리게이트를 생성함으로써 함수와 연동시킨 후
			넘겨주는 방식이 편리
			AssetStreamingHandle = MyGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMyCharacter::OnAssetLoadCompleted));
		}
	} */


	/*캐릭터는 자신이 주인공인지 NPC인지 판별할 수 있어야 하는데, 이를 확실히 파악할 수 있는 시점 중 하나는 BeginPlay이다
	현재 설정상 플레이어도 전용 AI컨트롤러가 자동으로 부착되므로 PossessedBy함수가 두번 호출되기 때문이다.
	캐릭터는 PREINIT 스테이트에서 시작한다. 그러다가 게임이 시작돼 BeginPlay함수를 호출하면 플레이어인지, NPC인지 파악해
	저장하는 변수 IsPlayer에 이 결과를 저장한다. 이어서 선택된 캐릭터 애셋의 로딩을 시작하고 LOADING으로 변경한다.*/
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

	//INI에서 불러옴
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

		//플레이어가 빙의할때 발생하는 캐릭터의 PossessedBy함수는 제거하고 대신 캐릭터의 READY스테이트에서 구현
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

//무기를 장착할수 있는지 확인
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

	//확인
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//소유권 지정(자기자신)
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

	//위아래, 좌우 설정
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	//공격설정
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

	//처음 시작할때 READY상태로 시작
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
	//델리게이트 추가
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

	//캐릭터스탯에서 선언한 델리게이트를 람다식으로 바인딩
	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

//데미지를 입히는곳
float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	
	//SetDamage함수호출
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
//캐릭터에게 몽타주를 사용하여 공격 애니메이션을 재생하라는 명령을 내림
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
//델리게이트
void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	//공격이 끝나면 알려줌
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
	//검색하여 자신은 탐색이 되지 않도록 설정
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
			//50씩
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}