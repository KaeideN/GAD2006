#include "ANetPlayerState.h"
#include "ANetBaseCharacter.h"
#include "Net/UnrealNetwork.h"

ANetPlayerState::ANetPlayerState()
{
	bReplicates = true;
}

void ANetPlayerState::OnRep_Data()
{
	if (ANetBaseCharacter* Character = Cast<ANetBaseCharacter>(GetPawn()))
	{
		Character->ApplyPlayerInfo(Data);
	}
}

void ANetPlayerState::ServerSetPlayerInfo_Implementation(FSPlayerInfo Info)
{
	Data = Info;

	if (HasAuthority())
	{
		OnRep_Data();
	}
}

void ANetPlayerState::SetTeam(EPlayerTeam Team)
{
	if (HasAuthority())
	{
		Data.TeamID = Team;
		OnRep_Data();
	}
}

void ANetPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetPlayerState, Data);
}
