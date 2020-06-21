// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "MyGameInstance.generated.h"

//MyCharacterData라는 GameData 구조체 선언
USTRUCT(BlueprintType)
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public :
	FMyCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

/**
 * 
 */
UCLASS()
class PROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public :
	UMyGameInstance();
	//해당 데이터가 잘 로딩됐는지 파악할 수 있도록 20레벨의 데이터를 출력하는 코드 추가
	virtual void Init() override;
	FMyCharacterData* GetMyCharacterData(int32 Level);
	
	//비동기 방식으로 애셋을 로딩할수 있게해주는 클래스 프로젝트에서 하나만 활성화하는것이 좋다.
	//UAssetManager라는 오브젝트에 이미 선언되어 있으므로 Enine/AssetManager.h헤더 파일을 포함한 후 UAssetManager::GetStreamableManager()함수
	FStreamableManager StreamableManager;

private :
	//DataTable이라는 언리얼 오브젝트 제공
	UPROPERTY()
	class UDataTable* MyCharacterTable;

};
