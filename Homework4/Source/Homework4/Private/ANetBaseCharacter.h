#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "ANetBaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	BP_Face = 0,
	BP_Hair = 1,
	BP_Beard = 2,
	BP_Eyebrows = 3,
	BP_Chest = 4,
	BP_Legs = 5,
	BP_Hands = 6,
	BP_COUNT = 7,
};

USTRUCT(BlueprintType)
struct FSMeshAssetList : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USkeletalMesh*> ListSkeletal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> ListStatic;
};

USTRUCT(BlueprintType)
struct FSBodyPartSelection
{
	GENERATED_BODY()

	int Indices[(int)EBodyPart::BP_COUNT];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFemale;
};

USTRUCT(BlueprintType)
struct FSPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSBodyPartSelection BodyParts;

	bool Ready;
};

UCLASS()
class HOMEWORK4_API ANetBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANetBaseCharacter();

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ChangeBodyPart(EBodyPart index, int value, bool DirectSet);

	UFUNCTION(BlueprintCallable)
	void ChangeGender(bool isFemale);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerInfoChanged)
	FSBodyPartSelection PartSelection;

	UFUNCTION(Server, Reliable)
	void SubmitPlayerInfoToServer(FSPlayerInfo Info);

	UFUNCTION()
	void OnRep_PlayerInfoChanged();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	USkeletalMeshComponent* PartFace;

	UPROPERTY()
	UStaticMeshComponent* PartHair;

	UPROPERTY()
	UStaticMeshComponent* PartBeard;

	UPROPERTY()
	UStaticMeshComponent* PartEyebrows;

	UPROPERTY()
	UStaticMeshComponent* PartEyes;

	UPROPERTY()
	USkeletalMeshComponent* PartHands;

	UPROPERTY()
	USkeletalMeshComponent* PartLegs;

	static FSMeshAssetList* GetBodyPartList(EBodyPart part, bool isFemale);
	void UpdateBodyParts();
	void ApplyGenderCorrections();
};
