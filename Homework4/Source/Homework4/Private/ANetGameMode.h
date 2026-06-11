#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ANetPlayerState.h"
#include "ANetGameMode.generated.h"

class ANetAvatar;

UCLASS()
class HOMEWORK4_API ANetGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANetGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	void AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB);

	void HandleBlueTeamVictory();
	void HandleRedTeamVictory(ANetPlayerState* RedPlayer);

protected:
	void SwapTeamPositions(ANetPlayerState* BluePlayer, ANetPlayerState* RedPlayer);

private:
	ANetPlayerState* FindPlayerByTeam(EPlayerTeam Team) const;
};
