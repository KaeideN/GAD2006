#include "MoveCommand.h"
#include "GameGrid.h"
#include "UnitBase.h"

MoveCommand::MoveCommand(FSGridPosition Src, FSGridPosition Dst) :
	Source(Src),
	Destination(Dst)
{
}

MoveCommand::~MoveCommand()
{
}

void MoveCommand::Execute()
{
	UE_LOG(LogTemp, Warning, TEXT("Executing MoveCommand..."));
	AGameSlot* SlotA = AGameGrid::FindSlot(Source);
	AGameSlot* SlotB = AGameGrid::FindSlot(Destination);

	AUnitBase* UnitA = SlotA->Unit;
	check(UnitA);
	UnitA->AssignToSlot(SlotB);
	SlotB->SetState(GS_Highlighted);
}

void MoveCommand::Revert()
{
	AGameSlot* SlotA = AGameGrid::FindSlot(Source);
	AGameSlot* SlotB = AGameGrid::FindSlot(Destination);

	if (SlotB && SlotB->Unit)
	{
		SlotB->Unit->AssignToSlot(SlotA);
		SlotB->SetState(GS_Default);
	}
}
