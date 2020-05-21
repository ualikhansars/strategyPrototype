// Fill out your copyright notice in the Description page of Project Settings.

#include "SP_PlayerController.h"

ASP_PlayerController::ASP_PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ASP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_PlayerController::FindPlayerPawn()
{
	if (PlayerPawn != nullptr)
		return;

	TSubclassOf<ASP_Player> ClassToFind;
	ClassToFind = ASP_Player::StaticClass();
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundPlayers);
	if (FoundPlayers.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player pawn found"));
		return;
	}

	PlayerPawn = Cast<ASP_Player>(FoundPlayers[0]);
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast AActor to SP_Player in PlayerController"));
	}
}

void ASP_PlayerController::LeftMousePressed()
{
	FHitResult LeftMouseHitResult;
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, LeftMouseHitResult);
	UE_LOG(LogTemp, Warning, TEXT("LeftMousePressed"));
	UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f, Z: %f"), LeftMouseHitResult.Location.X, LeftMouseHitResult.Location.Y, LeftMouseHitResult.Location.Z);

	FindPlayerPawn();

	if (PlayerPawn != nullptr)
	{
		PlayerPawn->MoveToLocation(LeftMouseHitResult.Location);
	}
}

void ASP_PlayerController::RightMousePressed()
{

}

void ASP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ASP_PlayerController::LeftMousePressed);
	InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &ASP_PlayerController::RightMousePressed);
}

