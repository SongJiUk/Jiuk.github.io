// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterSelectWidget.h"
#include "MyCharacterSetting.h"
#include "MyGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "MySaveGame.h"
#include "MyPlayerState.h"

void UMyCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<UMyCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto MyGameInstance = GetWorld()->GetGameInstance<UMyGameInstance>();
	ABCHECK(nullptr != MyGameInstance);
	ABCHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = MyGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void UMyCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UMyCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	//현재 월드에 있는 특정 타입을 상속받은 액터의 목록은 TActorIterator<액터타입> 구문을 사용해 가져올 수 있다.
	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &UMyCharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UMyCharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UMyCharacterSelectWidget::OnConfirmClicked);
}

void UMyCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UMyCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UMyCharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() >= 10) return;

	UMySaveGame* NewPlayerData = NewObject<UMySaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto MyPlayerState = GetDefault<AMyPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, MyPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GamePlay"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}