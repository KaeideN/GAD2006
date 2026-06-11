#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "ANetPlayerState.h"
#include "ANetBaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	BP_BodyType = 0,
	BP_Face = 1,
	BP_Hair = 2,
	BP_Beard = 3,
	BP_Eyebrows = 4,
	BP_Chest = 5,
	BP_Legs = 6,
	BP_Hands = 7,
	BP_COUNT = 8,
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

	UFUNCTION(BlueprintCallable)
	FString GetCustomizationData() const;

	UFUNCTION(BlueprintCallable)
	void ParseCustomizationData(const FString& BodyPartData);

	UFUNCTION(BlueprintCallable)
	void ApplyPlayerInfo(const FSPlayerInfo& Info);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsFemale() const;

	int32 BodyPartIndices[(int)EBodyPart::BP_COUNT];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PartHands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PartLegs;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInfoChanged();

protected:
	void CheckPlayerInfo();

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

	FTimerHandle ClientDataCheckTimer;

	static FSMeshAssetList* GetBodyPartList(EBodyPart part, bool isFemale);
	void UpdateBodyParts();
	void ApplyGenderCorrections();
};
