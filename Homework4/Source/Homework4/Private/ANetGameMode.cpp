#include "ANetGameMode.h"
#include "ANetAvatar.h"
#include "ANetGameState.h"
#include "ANetPlayerState.h"
#include "GameFramework/PlayerState.h"

ANetGameMode::ANetGameMode()
{
	DefaultPawnClass = ANetAvatar::StaticClass();
	PlayerStateClass = ANetPlayerState::StaticClass();
	GameStateClass = ANetGameState::StaticClass();
}

void ANetGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!HasAuthority() || !NewPlayer)
	{
		return;
	}

	ANetPlayerState* PlayerState = NewPlayer->GetPlayerState<ANetPlayerState>();
	if (!PlayerState)
	{
		return;
	}

	// First connected player is Blue (survivor), everyone else is Red (chaser).
	const EPlayerTeam AssignedTeam = (GetNumPlayers() <= 1)
		? EPlayerTeam::TEAM_Blue
		: EPlayerTeam::TEAM_Red;
	PlayerState->SetTeam(AssignedTeam);
}

void ANetGameMode::AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB)
{
	if (!HasAuthority() || !AvatarA || !AvatarB)
	{
		return;
	}

	ANetPlayerState* StateA = AvatarA->GetPlayerState<ANetPlayerState>();
	ANetPlayerState* StateB = AvatarB->GetPlayerState<ANetPlayerState>();
	if (!StateA || !StateB)
	{
		return;
	}

	const EPlayerTeam TeamA = StateA->Data.TeamID;
	const EPlayerTeam TeamB = StateB->Data.TeamID;

	if ((TeamA == EPlayerTeam::TEAM_Red && TeamB == EPlayerTeam::TEAM_Blue)
		|| (TeamA == EPlayerTeam::TEAM_Blue && TeamB == EPlayerTeam::TEAM_Red))
	{
		ANetPlayerState* RedPlayer = (TeamA == EPlayerTeam::TEAM_Red) ? StateA : StateB;
		HandleRedTeamVictory(RedPlayer);
	}
}

void ANetGameMode::HandleBlueTeamVictory()
{
	ANetPlayerState* BluePlayer = FindPlayerByTeam(EPlayerTeam::TEAM_Blue);
	ANetPlayerState* RedPlayer = FindPlayerByTeam(EPlayerTeam::TEAM_Red);
	if (!BluePlayer)
	{
		return;
	}

	// Swap positions so the surviving blue player takes the chaser spawn and vice versa.
	if (RedPlayer)
	{
		SwapTeamPositions(BluePlayer, RedPlayer);
	}

	if (ANetGameState* NetGameState = GetGameState<ANetGameState>())
	{
		NetGameState->TriggerVictory(BluePlayer->GetPlayerId());
	}
}

void ANetGameMode::HandleRedTeamVictory(ANetPlayerState* RedPlayer)
{
	if (!RedPlayer)
	{
		return;
	}

	if (ANetGameState* NetGameState = GetGameState<ANetGameState>())
	{
		NetGameState->TriggerVictory(RedPlayer->GetPlayerId());
	}
}

void ANetGameMode::SwapTeamPositions(ANetPlayerState* BluePlayer, ANetPlayerState* RedPlayer)
{
	if (!BluePlayer || !RedPlayer)
	{
		return;
	}

	APawn* BluePawn = BluePlayer->GetPawn();
	APawn* RedPawn = RedPlayer->GetPawn();
	if (!BluePawn || !RedPawn)
	{
		return;
	}

	// Chosen logic: swap world transforms between the blue survivor and the first red chaser found.
	const FTransform BlueTransform = BluePawn->GetActorTransform();
	BluePawn->SetActorTransform(RedPawn->GetActorTransform());
	RedPawn->SetActorTransform(BlueTransform);
}

ANetPlayerState* ANetGameMode::FindPlayerByTeam(EPlayerTeam Team) const
{
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (ANetPlayerState* NetPlayerState = Cast<ANetPlayerState>(PlayerState))
		{
			if (NetPlayerState->Data.TeamID == Team)
			{
				return NetPlayerState;
			}
		}
	}

	return nullptr;
}
