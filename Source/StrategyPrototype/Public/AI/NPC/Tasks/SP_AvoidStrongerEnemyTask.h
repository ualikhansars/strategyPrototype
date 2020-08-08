#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SP_AvoidStrongerEnemyTask.generated.h"

UCLASS()
class STRATEGYPROTOTYPE_API USP_AvoidStrongerEnemyTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	USP_AvoidStrongerEnemyTask(const FObjectInitializer& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
