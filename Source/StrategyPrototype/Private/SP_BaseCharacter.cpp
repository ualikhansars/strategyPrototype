#include "SP_BaseCharacter.h"
#include "Utils/EventSystem/Events/SP_FightEvent.h"

ASP_BaseCharacter::ASP_BaseCharacter()
{
	Tags.Add(FName("Character"));

	CharacterCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CharacterCollisionComp"));
	CharacterCollisionComp->SetGenerateOverlapEvents(true);
	CharacterCollisionComp->SetupAttachment(RootComponent);
	CharacterCollisionComp->InitCapsuleSize(88.0f, 80.0f);
	CharacterCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASP_BaseCharacter::OnOverlapBegin);
	CharacterCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ASP_BaseCharacter::OnOverlapEnd);
	
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));

	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = MAX_WALK_SPEED_DEFAULT;

	SetMode(SP_CharacterMode::Roaming);
}

void ASP_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_BaseCharacter::SetMode(SP_CharacterMode NewMode)
{
	Mode = NewMode;
}

void ASP_BaseCharacter::StopMovement()
{
	CharacterMovementComp->StopMovementImmediately();
}

void ASP_BaseCharacter::InteractWithCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("Character is overlapping with other characters"));
}

SP_CharacterMode ASP_BaseCharacter::GetMode()
{
	return Mode;
}

void ASP_BaseCharacter::AddItem(FSP_Item& Item)
{
	if (ActorHasTag("Player"))
	{
		Item.SetOwner(SP_ItemOwner::Player);
	}
	else if(ActorHasTag("NPC"))
	{
		Item.SetOwner(SP_ItemOwner::NPC);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Charactor should have tag Player or NPC"));
		return;
	}

	Goods.Add(Item);
	Food += Item.NutritionalValue;
}

void ASP_BaseCharacter::RemoveItem(const FSP_Item& Item)
{
	Goods.RemoveSingle(Item);
	Food -= Item.NutritionalValue;
}

FName ASP_BaseCharacter::GetName() const
{
	return CharacterName;
}

void ASP_BaseCharacter::AddGold(float Value)
{
	Gold += Value;
}

float ASP_BaseCharacter::GetGold() const
{
	return Gold;
}

TArray<FSP_Item> ASP_BaseCharacter::GetGoods() const
{
	return Goods;
}

void ASP_BaseCharacter::ConsumeFood()
{
	for (FSP_Unit Unit : Units)
	{
		float Hunger = Unit.FoodConsumption;
		int CurrentIndex = 0;
		while (Hunger > 0.0f && CurrentIndex < Goods.Num())
		{
			if (Goods[CurrentIndex].Consumable)
			{
				if (Goods[CurrentIndex].NutritionalValue >= Hunger)
				{
					Goods[CurrentIndex].NutritionalValue -= Hunger;
					Food -= Hunger;
					Hunger = 0.0f;
				}
				else
				{
					RemoveItem(Goods[CurrentIndex]);
					Food -= Goods[CurrentIndex].NutritionalValue;
					Hunger -= Goods[CurrentIndex].NutritionalValue;
				}
				
			}

			CurrentIndex++;
		}
	}
}

void ASP_BaseCharacter::PayUnitsWage()
{
	Gold -= UnitPayment;
}

void ASP_BaseCharacter::Init(FName Name)
{
	CharacterName = Name;
}

void ASP_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASP_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASP_BaseCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag("Road"))
		{
			CharacterMovementComp->MaxWalkSpeed = MAX_WALK_SPEED_RODE;
		}
		else if (OtherActor->ActorHasTag("Swamp"))
		{
			CharacterMovementComp->MaxWalkSpeed = MAX_WALK_SPEED_SWAMP;
		}

		if (OtherActor->ActorHasTag("Character"))
		{
			ASP_BaseCharacter* OtherCharacter = Cast<ASP_BaseCharacter>(OtherActor);
			if (OtherCharacter != nullptr)
			{
				if (Mode == SP_CharacterMode::Attacking || OtherCharacter->Mode == SP_CharacterMode::Attacking)
				{
					UE_LOG(LogTemp, Warning, TEXT("Fight is starting"));
					StartBattle(OtherCharacter);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to cast OtherActor to ASP_BaseCharacter"));
			}
		}
	}
}

void ASP_BaseCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag("NavigationArea"))
			CharacterMovementComp->MaxWalkSpeed = MAX_WALK_SPEED_DEFAULT;
	}
}

void ASP_BaseCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASP_BaseCharacter* OtherCharacter = Cast<ASP_BaseCharacter>(OtherActor);
	if (OtherActor && (OtherActor != this) && OtherCharacter)
	{
		if (!OtherCharacter->ActorHasTag("Player") && Mode == SP_CharacterMode::GoingToCharacter && OtherCharacter == CharacterToMove)
		{
			InteractWithCharacter();
		}
		OverlappingCharacter = OtherCharacter;
	}
}

void ASP_BaseCharacter::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ASP_BaseCharacter* OtherCharacter = Cast<ASP_BaseCharacter>(OtherActor);
	if (OtherActor && (OtherActor != this) && OtherCharacter)
	{
		OverlappingCharacter = nullptr;
	}
}

void ASP_BaseCharacter::StartBattle(ASP_BaseCharacter* OtherCharacter)
{
	if(Mode == SP_CharacterMode::Fighting || OtherCharacter->Mode == SP_CharacterMode::Fighting)
		return;
	StopMovement();
	OtherCharacter->StopMovement();

	SetMode(SP_CharacterMode::Fighting);
	OtherCharacter->SetMode(SP_CharacterMode::Fighting);

	if (ActorHasTag("Player"))
	{
		OtherCharacter->Destroy();
	}
	
	SP_FightEvent FightEvent;
	Dispatch(SP_EventType::FightOccured, FightEvent);
	UE_LOG(LogTemp, Warning, TEXT("Fight is over"));
}

void ASP_BaseCharacter::AddUnit(const FSP_Unit& Unit)
{
	Units.Add(Unit);
	UnitPayment += Unit.Payment;
	FoodConsumption += Unit.FoodConsumption;
}

void ASP_BaseCharacter::AttackUnit(FSP_Unit* AttackUnit, FSP_Unit* DefendUnit)
{
	if (AttackUnit->Health > 0)
	{
		DefendUnit->Health -= AttackUnit->Damage;
	}
}
