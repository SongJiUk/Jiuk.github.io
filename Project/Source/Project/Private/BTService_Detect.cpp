// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}
/*NPC의 위치를 기준으로 반경6미터 내에 캐릭터가 있는지 감시하는 기능을 넣는다.
반경 내에 다른 NPC MyCharacter가 있는 경우도 가정해 반경 내 모든 캐릭터를 감지하는 OverlapMultiByChannel 함수를 사용한다.
반경 내에 감지된 모든 캐릭터 정보는 목록을 관리하는데 적합한 언리얼 엔진의 자료구조인 TArray로 전달된다.
*/
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* CotrollingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == CotrollingPawn) return;

	UWorld* World = CotrollingPawn->GetWorld();
	FVector Center = CotrollingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, CotrollingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
		);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AMyCharacter* MyCharacter = Cast<AMyCharacter>(OverlapResult.GetActor());
			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, MyCharacter);
				return;
			}
		}
	}
	//플레이어 캐릭터가 감지되면 블랙보드의 Target값을 플레이어 캐릭터로 지정하고, 그렇지 않으면 nullptr값을 지정한다.
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);
		
}