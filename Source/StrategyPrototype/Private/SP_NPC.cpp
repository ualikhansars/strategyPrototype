#include "SP_NPC.h"
#include "Kismet/GameplayStatics.h"

ASP_NPC::ASP_NPC()
{
	Tags.Add(FName("NPC"));

	FSP_Unit Cavalry = USP_UnitFactory::CreateUnit(SP_UnitType::Cavalry);
	Units.Add(Cavalry);

	FSP_Item Item1 = USP_ItemFactory::CreateItem(SP_ItemType::Bread, SP_ItemOwner::NPC);
	FSP_Item Item2 = USP_ItemFactory::CreateItem(SP_ItemType::Sword, SP_ItemOwner::NPC);
	FSP_Item Item3 = USP_ItemFactory::CreateItem(SP_ItemType::Necklace, SP_ItemOwner::NPC);

	AddItem(Item1);
	AddItem(Item2);
	AddItem(Item3);
}

void ASP_NPC::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASP_NPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASP_NPC::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Town"))
	{

	}
}

float ASP_NPC::GetDailyFoodConsumption() const
{
	int FoodConsumptionPerDay = 3;
	return FoodConsumption * FoodConsumptionPerDay;
}

void ASP_NPC::MoveToTown(AActor* Town)
{
	SetMode(SP_CharacterMode::GoingToTown);
}

