// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

UCLASS()
class PROJECT_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UPROPERTY(VisibleAnywhere, Category=Box)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;

	//특정 클래스와 상속받은 클래스들로 목록을 한정하도록 키워드 제공
	//목록에서 아이템 상자와 이를 선언한 클래스 목록만 볼 수 있다.
	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class AMyWeapon> WeaponItemClass;


private :
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//멤버함수를 하나 추가하고 파티클 컴포넌트 시스템에서 제공하는 OnSystemFinished델리게이트에 이를 연결해
	//이펙트 재생이 종료되면 아이템 상자가 제거되도록 로직을 구성 
	//이때 OnSystemFinished델리게이트는 다이내믹 형식이므로 UFUNCTION을 해주어야한다.
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);
};
 