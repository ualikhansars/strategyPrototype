#include "World/WorldState/SP_WorldState.h"

uint8 USP_WorldState::mStability = 0;
uint8 USP_WorldState::mStarvation = 0;

uint8 USP_WorldState::GetStability()
{
	return mStability;
}

uint8 USP_WorldState::GetStarvation()
{
	return mStarvation;
}
