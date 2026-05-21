// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSlot.h"
#include "UnitBase.h"
#include "TBPlayerController.h"

AGameSlot::AGameSlot()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(50, 50, 2));
	Box->SetCollisionResponseToAllChannels(ECR_Block);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(RootComponent);
	Plane->SetStaticMesh(DefaultSlotMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		DefaultSlotMaterialFinder(TEXT("Material'/Game/Materials/MAT_GridSlot.MAT_GridSlot'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		DefaultSlotMaterial_HFinder(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Highlighted.MAT_GridSlot_Highlighted'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		DefaultSlotMaterial_OFinder(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Offensive.MAT_GridSlot_Offensive'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		DefaultSlotMaterial_SFinder(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Supportive.MAT_GridSlot_Supportive'"));

	DefaultSlotMaterial = DefaultSlotMaterialFinder.Object;
	DefaultSlotMaterial_H = DefaultSlotMaterial_HFinder.Object;
	DefaultSlotMaterial_O = DefaultSlotMaterial_OFinder.Object;
	DefaultSlotMaterial_S = DefaultSlotMaterial_SFinder.Object;

	SetState(GS_Default);
}

void AGameSlot::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddUniqueDynamic(this, &AGameSlot::OnGridClicked);
}

void AGameSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameSlot::SetState(EGridState NewState)
{
	GridState = NewState;

	switch (NewState)
	{
	case GS_Default:
		Plane->SetMaterial(0, DefaultSlotMaterial);
		break;
	case GS_Highlighted:
		Plane->SetMaterial(0, DefaultSlotMaterial_H);
		break;
	case GS_Offensive:
		Plane->SetMaterial(0, DefaultSlotMaterial_O);
		break;
	case GS_Supportive:
		Plane->SetMaterial(0, DefaultSlotMaterial_S);
		break;
	}
}

void AGameSlot::SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass)
{
	FVector Location = GetActorLocation();
	AUnitBase* NewUnit = Cast<AUnitBase>(GWorld->SpawnActor(UnitClass, &Location));
	if (NewUnit) NewUnit->AssignToSlot(this);
}

void AGameSlot::OnGridClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (auto PlayerController = GWorld->GetFirstPlayerController<ATBPlayerController>())
	{
		PlayerController->OnActorClicked(this, ButtonPressed);
	}
}
