// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "MyWeapon.h"
#include "MyCharacter.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/ModernCityEnvironment01/Meshes/SM_MTrashCan.SM_MTrashCan"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_OPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_OPEN.Succeeded())
	{
		Effect->SetTemplate(P_OPEN.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	//생성자 코드에서 해당 속성에 대한 기본 클래스 값을 지정
	WeaponItemClass = AMyWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnCharacterOverlap);
}

void AItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	auto MyCharacter = Cast<AMyCharacter>(OtherActor);
	ABCHECK(nullptr != MyCharacter);

	if (nullptr != MyCharacter && nullptr != WeaponItemClass)
	{
		if (MyCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			MyCharacter->SetWeapon(NewWeapon);
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &AItemBox::OnEffectFinished);
		}
		else
		{
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *MyCharacter->GetName());
		}
	}
}

void AItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}