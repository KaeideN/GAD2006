// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class ACOAAvatar : public ACOABaseCharacter
{
	GENERATED_BODY()

public:
	ACOAAvatar();

	UCameraComponent* Camera;
	USpringArmComponent* SpringArm;

	// Stamina System Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaGainRate = 15.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	bool bStaminaDrained = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 600.0f;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Tick override for stamina updates
	virtual void Tick(float DeltaTime) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RunPressed();
	void RunReleased();

	/**
	 * Updates movement speed based on stamina state.
	 * If stamina is drained, player is forced to walk speed.
	 * Otherwise, respects the current running state.
	 */
	void UpdateMovementParams();

};
