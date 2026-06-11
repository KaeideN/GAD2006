#pragma once

#include "CoreMinimal.h"
#include "ANetBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ANetAvatar.generated.h"

UCLASS()
class HOMEWORK4_API ANetAvatar : public ANetBaseCharacter
{
	GENERATED_BODY()

public:
	ANetAvatar();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(ReplicatedUsing = OnRep_IsRunning)
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();

	UFUNCTION(Server, Reliable)
	void ServerSetRunning(bool bNewRunning);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	float WalkSpeed;
	float RunSpeed;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();
	void UpdateMovementSpeed();
};
