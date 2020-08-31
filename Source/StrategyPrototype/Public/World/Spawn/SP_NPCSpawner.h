#pragma once

#include "CoreMinimal.h"
#include "SP_NPC.h"

class STRATEGYPROTOTYPE_API SP_NPCSpawner
{
public:
	void SpawnNPC(UWorld* World, TSubclassOf<ASP_NPC> NPCClass);
};