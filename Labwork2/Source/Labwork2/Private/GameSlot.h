// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameSlot.generated.h"

USTRUCT(Blueprintable)
struct FSGridPosition
{
	GENERATED_USTRUCT_BODY();

	FSGridPosition() {}
	FSGridPosition(int col, int row) : Col(col), Row(row) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Col;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Row;
};

UENUM(Blueprintable)
enum EGridState
{
	GS_Default,
	GS_Highlighted,
	GS_Offensive,
	GS_Supportive,
};

class AUnitBase;

UCLASS()
class AGameSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameSlot();

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Plane;

	UPROPERTY(BlueprintReadWrite)
	FSGridPosition GridPosition;

	UFUNCTION()
	void SetState(EGridState NewState);

	UPROPERTY(VisibleAnywhere)
	AUnitBase* Unit;

	void SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass);

protected:
	virtual void BeginPlay() override;

private:
	EGridState GridState;

	UFUNCTION()
	void OnGridClicked(AActor* TouchedActor, FKey ButtonPressed);

	UMaterialInterface* DefaultSlotMaterial;
	UMaterialInterface* DefaultSlotMaterial_H;
	UMaterialInterface* DefaultSlotMaterial_O;
	UMaterialInterface* DefaultSlotMaterial_S;

public:	
	virtual void Tick(float DeltaTime) override;

};
