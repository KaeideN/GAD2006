#include "ANetGameState.h"
#include "ANetGameMode.h"
#include "TimerManager.h"

ANetGameState::ANetGameState()
{
	bReplicates = true;
}

void ANetGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		StartBlueTeamTimer();
	}
}

void ANetGameState::StartBlueTeamTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		BlueTeamTimerHandle,
		this,
		&ANetGameState::OnBlueTeamTimerExpired,
		BlueTeamTimeLimit,
		false);
}

void ANetGameState::StopBlueTeamTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(BlueTeamTimerHandle);
}

void ANetGameState::OnBlueTeamTimerExpired()
{
	if (ANetGameMode* GameMode = GetWorld()->GetAuthGameMode<ANetGameMode>())
	{
		GameMode->HandleBlueTeamVictory();
	}
}

void ANetGameState::TriggerVictory(int32 WinnerPlayerId)
{
	if (!HasAuthority())
	{
		return;
	}

	StopBlueTeamTimer();
	WinningPlayer = WinnerPlayerId;
	OnVictory();
}
