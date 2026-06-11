#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ANetGameState.generated.h"

UCLASS()
class HOMEWORK4_API ANetGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ANetGameState();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	int32 WinningPlayer = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BlueTeamTimeLimit = 30.f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnVictory();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnRestart();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void ShowResultMessage();

	void StopBlueTeamTimer();
	void TriggerVictory(int32 WinnerPlayerId);

protected:
	void StartBlueTeamTimer();
	void OnBlueTeamTimerExpired();

private:
	FTimerHandle BlueTeamTimerHandle;
};
