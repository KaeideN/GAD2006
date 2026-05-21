// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitBase.h"
#include "GameSlot.h"

AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitBase::AssignToSlot(AGameSlot* NewSlot)
{
	check(NewSlot && NewSlot->Unit == nullptr);

	if (Slot) Slot->Unit = nullptr;

	Slot = NewSlot;

	Slot->Unit = this;

	SetActorLocation(Slot->GetActorLocation() + StartOffset);
}
