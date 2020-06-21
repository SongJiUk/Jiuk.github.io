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
/*NPC�� ��ġ�� �������� �ݰ�6���� ���� ĳ���Ͱ� �ִ��� �����ϴ� ����� �ִ´�.
�ݰ� ���� �ٸ� NPC MyCharacter�� �ִ� ��쵵 ������ �ݰ� �� ��� ĳ���͸� �����ϴ� OverlapMultiByChannel �Լ��� ����Ѵ�.
�ݰ� ���� ������ ��� ĳ���� ������ ����� �����ϴµ� ������ �𸮾� ������ �ڷᱸ���� TArray�� ���޵ȴ�.
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
	//�÷��̾� ĳ���Ͱ� �����Ǹ� �������� Target���� �÷��̾� ĳ���ͷ� �����ϰ�, �׷��� ������ nullptr���� �����Ѵ�.
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);
		
}