#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ANetBaseCharacter.h"
#include "ANetGameInstance.generated.h"

UCLASS()
class HOMEWORK4_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Host(FString MapName, FSPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable)
	void Join(FString Address, FSPlayerInfo& PlayerInfo);

	UPROPERTY(BlueprintReadWrite)
	FSPlayerInfo PlayerInfo;
};
