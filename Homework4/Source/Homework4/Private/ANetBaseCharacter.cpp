#include "ANetBaseCharacter.h"
#include "ANetGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

static UDataTable* SBodyParts = nullptr;

static const TArray<FString> BodyPartNames
{
	TEXT("Face"),
	TEXT("Hair"),
	TEXT("Beard"),
	TEXT("Eyebrows"),
	TEXT("Chest"),
	TEXT("Legs"),
	TEXT("Hands")
};

ANetBaseCharacter::ANetBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PartFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	PartFace->SetupAttachment(GetMesh());
	PartFace->SetLeaderPoseComponent(GetMesh());

	PartHands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	PartHands->SetupAttachment(GetMesh());
	PartHands->SetLeaderPoseComponent(GetMesh());

	PartLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	PartLegs->SetupAttachment(GetMesh());
	PartLegs->SetLeaderPoseComponent(GetMesh());

	PartHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	PartHair->SetupAttachment(PartFace, FName("headSocket"));

	PartBeard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beard"));
	PartBeard->SetupAttachment(PartFace, FName("headSocket"));

	PartEyebrows = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyebrows"));
	PartEyebrows->SetupAttachment(PartFace, FName("headSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_Eyes(TEXT("StaticMesh'/Game/StylizedModularChar/Meshes/SM_Eyes.SM_Eyes'"));
	PartEyes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	PartEyes->SetupAttachment(PartFace, FName("headSocket"));
	if (SK_Eyes.Succeeded())
	{
		PartEyes->SetStaticMesh(SK_Eyes.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BodyParts(TEXT("DataTable'/Game/Blueprints/DT_BodyParts.DT_BodyParts'"));
	if (DT_BodyParts.Succeeded())
	{
		SBodyParts = DT_BodyParts.Object;
	}
}

void ANetBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		UNetGameInstance* Instance = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
		if (Instance && Instance->PlayerInfo.Ready)
		{
			SubmitPlayerInfoToServer(Instance->PlayerInfo);
		}
	}
}

void ANetBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANetBaseCharacter::ChangeBodyPart(EBodyPart index, int value, bool DirectSet)
{
	if (PartSelection.isFemale && index == EBodyPart::BP_Beard)
	{
		ApplyGenderCorrections();
		return;
	}

	FSMeshAssetList* List = GetBodyPartList(index, PartSelection.isFemale);
	if (!List)
	{
		return;
	}

	int CurrentIndex = PartSelection.Indices[(int)index];
	if (DirectSet)
	{
		CurrentIndex = value;
	}
	else
	{
		CurrentIndex += value;
	}

	const int Num = List->ListSkeletal.Num() + List->ListStatic.Num();
	if (Num > 0)
	{
		while (CurrentIndex < 0)
		{
			CurrentIndex += Num;
		}
		CurrentIndex %= Num;
	}

	PartSelection.Indices[(int)index] = CurrentIndex;

	if (CurrentIndex < List->ListSkeletal.Num())
	{
		switch (index)
		{
		case EBodyPart::BP_Face:
			PartFace->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]);
			break;
		case EBodyPart::BP_Chest:
			GetMesh()->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]);
			break;
		case EBodyPart::BP_Hands:
			PartHands->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]);
			break;
		case EBodyPart::BP_Legs:
			PartLegs->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]);
			break;
		default:
			break;
		}
	}
	else
	{
		const int StaticIndex = CurrentIndex - List->ListSkeletal.Num();
		switch (index)
		{
		case EBodyPart::BP_Beard:
			PartBeard->SetStaticMesh(List->ListStatic[StaticIndex]);
			break;
		case EBodyPart::BP_Hair:
			PartHair->SetStaticMesh(List->ListStatic[StaticIndex]);
			break;
		case EBodyPart::BP_Eyebrows:
			PartEyebrows->SetStaticMesh(List->ListStatic[StaticIndex]);
			break;
		default:
			break;
		}
	}

	ApplyGenderCorrections();
}

void ANetBaseCharacter::ChangeGender(bool _isFemale)
{
	PartSelection.isFemale = _isFemale;
	UpdateBodyParts();
}

void ANetBaseCharacter::UpdateBodyParts()
{
	ChangeBodyPart(EBodyPart::BP_Face, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hair, 0, false);
	ChangeBodyPart(EBodyPart::BP_Beard, 0, false);
	ChangeBodyPart(EBodyPart::BP_Eyebrows, 0, false);
	ChangeBodyPart(EBodyPart::BP_Chest, 0, false);
	ChangeBodyPart(EBodyPart::BP_Legs, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hands, 0, false);
	ApplyGenderCorrections();
}

void ANetBaseCharacter::ApplyGenderCorrections()
{
	if (PartSelection.isFemale)
	{
		PartBeard->SetStaticMesh(nullptr);
		PartBeard->SetVisibility(false);
	}
	else
	{
		PartBeard->SetVisibility(true);
	}
}

FSMeshAssetList* ANetBaseCharacter::GetBodyPartList(EBodyPart part, bool isFemale)
{
	const FString Name = FString::Printf(TEXT("%s%s"), isFemale ? TEXT("Female") : TEXT("Male"), *BodyPartNames[(int)part]);
	return SBodyParts ? SBodyParts->FindRow<FSMeshAssetList>(*Name, nullptr) : nullptr;
}

void ANetBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateBodyParts();
}

void ANetBaseCharacter::OnRep_PlayerInfoChanged()
{
	UpdateBodyParts();
}

void ANetBaseCharacter::SubmitPlayerInfoToServer_Implementation(FSPlayerInfo Info)
{
	PartSelection = Info.BodyParts;

	if (HasAuthority())
	{
		OnRep_PlayerInfoChanged();
	}
}

void ANetBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetBaseCharacter, PartSelection);
}
