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
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(ReplicatedUsing = OnRep_IsRunning, BlueprintReadOnly)
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();

	UFUNCTION(Server, Reliable)
	void ServerSetRunning(bool bNewRunning);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Defeat();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Victory();

	UFUNCTION(BlueprintCallable)
	void OnPlayerLost();

	UFUNCTION(BlueprintCallable)
	void OnPlayerWon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WalkSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RunSpeed = 600.f;

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();
	void UpdateMovementSpeed();
};
