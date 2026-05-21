// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGrid.h"
#include "Components/ChildActorComponent.h"

AGameGrid* AGameGrid::GameGrid = nullptr;

AGameGrid::AGameGrid() :
	NumRows(8),
	NumCols(8)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	GameGrid = this;
}

void AGameGrid::BeginPlay()
{
	Super::BeginPlay();

	int GridIndex = 0;

	for (int i = 0; i < NumRows; i++)
	{
		for (int j = 0; j < NumCols; j++)
		{
			AGameSlot* GameSlot = Cast<AGameSlot>(GridActors[GridIndex]->GetChildActor());
			if (GameSlot)
			{
				GameSlot->GridPosition = FSGridPosition(j, i);
			}
			GridIndex++;
		}
	}
}

void AGameGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AGameSlot* AGameGrid::GetSlot(FSGridPosition& Position)
{
	int GridIndex = Position.Row * NumCols + Position.Col;
	if (GridActors.IsValidIndex(GridIndex)) {
		return Cast<AGameSlot>(GridActors[GridIndex]->GetChildActor());
	}
	return nullptr;
}

AGameSlot* AGameGrid::FindSlot(FSGridPosition Position)
{
	if (GameGrid) {
		return GameGrid->GetSlot(Position);
	}

	return nullptr;
}

void AGameGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	for(auto Grid : GridActors)
	{
		if (Grid)
		{
			Grid->DestroyComponent();
		}
	}

	GridActors.Empty();

	if (!GridClass->IsValidLowLevel()) return;

	AGameSlot* Slot = GridClass->GetDefaultObject<AGameSlot>();

	if (Slot == nullptr) return;

	FVector Extends = Slot->Box->GetScaledBoxExtent()*2;

	for (int i = 0; i < NumRows; i++)
	{
		for (int j = 0; j < NumCols; j++)
		{
			FName GridName(FString::Printf(TEXT("Slot%dx%d"), j, i));
			
			auto Grid = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass(), GridName);
			Grid->RegisterComponent();
			Grid->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			
			Grid->SetRelativeLocation(
				FVector(
					(NumRows - i - 1) * Extends.X - (NumRows * 0.5f - 0.5f) * Extends.X,
					j * Extends.Y - (NumCols * 0.5f - 0.5f) * Extends.Y, 
					0
				)
			);

			GridActors.Add(Grid);

			Grid->SetChildActorClass(GridClass);
			AGameSlot* GameSlot = Cast<AGameSlot>(Grid->GetChildActor());
			if (GameSlot)
			{
				GameSlot->SetActorLabel(GridName.ToString());
			}
		}
	}
}
