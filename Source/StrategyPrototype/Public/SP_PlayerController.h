// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SP_Obstacle.h"
#include "UI/SP_HUD.h"
#include "SP_Player.h"
#include "SP_PlayerController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class SP_GameState : uint8
{
	Default,
	Inventory,
	Pause,
	Interaction
};

UCLASS()
class STRATEGYPROTOTYPE_API ASP_PlayerController : public APlayerController
{
	GENERATED_BODY()

	ASP_Player* PlayerPawn;

	void FindPlayerPawn();

	bool bPause = false;

	SP_GameState GameState = SP_GameState::Default;

	void PlayerNPCBuySellItem(FSP_Item DraggedItem, FSP_Item DroppedItem);

	void PlayerTownBuySellItem(FSP_Item DraggedItem, FSP_Item DroppedItem);

	ASP_HUD* HUD;

protected:
	void LeftMousePressed();

	void RightMousePressed();

public:
	ASP_PlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void OpenInventory();

	UFUNCTION(BlueprintCallable)
	void CloseInventory();

	UFUNCTION(BlueprintCallable)
	void DisplayNPCInteraction();

	UFUNCTION(BlueprintCallable)
	void HideDisplayInteraction();

	UFUNCTION(BlueprintCallable)
	void SetInputModeToGameOnly();

	UFUNCTION(BlueprintCallable)
	void BuySellItem(FSP_Item DraggedItem, FSP_Item DroppedItem);

	UFUNCTION(BlueprintCallable)
	void SetGameState(SP_GameState GameState);

	UFUNCTION(BlueprintCallable)
	SP_GameState GetGameState() const;

	UFUNCTION(BlueprintCallable)
	ASP_Player* GetPlayerPawn() const;
};
