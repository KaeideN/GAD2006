// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSlot.h"
#include "GameFramework/Actor.h"
#include "GameGrid.generated.h"

UCLASS()
class AGameGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameGrid();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameSlot> GridClass;

	UPROPERTY(EditAnywhere)
	int NumRows; 

	UPROPERTY(EditAnywhere)
	int NumCols;

	UPROPERTY(VisibleAnywhere)
	TArray<UChildActorComponent*> GridActors;

	AGameSlot* GetSlot(FSGridPosition& Position);
	static AGameSlot* FindSlot(FSGridPosition Position);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	static AGameGrid* GameGrid;

};
