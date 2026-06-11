#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ANetPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	TEAM_Unknown = 0,
	TEAM_Blue = 1,
	TEAM_Red = 2,
};

USTRUCT(BlueprintType)
struct FSPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CustomizationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerTeam TeamID = EPlayerTeam::TEAM_Unknown;

	UPROPERTY(BlueprintReadWrite)
	bool Ready = false;
};

UCLASS()
class HOMEWORK4_API ANetPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ANetPlayerState();

	UPROPERTY(ReplicatedUsing = OnRep_Data, BlueprintReadOnly)
	FSPlayerInfo Data;

	UFUNCTION()
	void OnRep_Data();

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerInfo(FSPlayerInfo Info);

	UFUNCTION(BlueprintCallable)
	void SetTeam(EPlayerTeam Team);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
