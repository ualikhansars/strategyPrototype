// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Objects/SP_Object.h"
#include "SP_Town.generated.h"

UCLASS()
class STRATEGYPROTOTYPE_API ASP_Town : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASP_Town();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MAX_MARKET_SLOTS = 40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSP_Unit> AvailableUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSP_Item> Goods;

	UFUNCTION(BlueprintCallable)
	void RemoveFromAvailableUnits(const FSP_Unit& Unit);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
};
