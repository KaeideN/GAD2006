// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"

ACOAAvatar::ACOAAvatar()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	// Create a follow camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	bUseControllerRotationYaw = true;

	// Initialize stamina to max
	Stamina = MaxStamina;
}

// Called to bind functionality to input
void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACOAAvatar::RunReleased);
}

void ACOAAvatar::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACOAAvatar::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACOAAvatar::RunPressed()
{
	// Only allow running if stamina is not drained
	if (!bStaminaDrained)
	{
		bRunning = true;
		UpdateMovementParams();
	}
}

void ACOAAvatar::RunReleased()
{
	bRunning = false;
	UpdateMovementParams();
}

void ACOAAvatar::UpdateMovementParams()
{
	if (!GetCharacterMovement())
	{
		return;
	}

	// If stamina is drained, force walk speed regardless of running state
	if (bStaminaDrained)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else if (bRunning)
	{
		// Running: use run speed
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		// Walking: use walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ACOAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only update stamina if character is walking
	if (GetCharacterMovement() && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		if (bRunning && !bStaminaDrained)
		{
			// Drain stamina while running
			Stamina -= StaminaDrainRate * DeltaTime;

			// Check if stamina reached zero
			if (Stamina <= 0.0f)
			{
				Stamina = 0.0f;
				bStaminaDrained = true;
				bRunning = false;
				UpdateMovementParams();
			}
		}
		else if (!bRunning || bStaminaDrained)
		{
			// Gain stamina while not running or while in drained state
			Stamina += StaminaGainRate * DeltaTime;

			// Clamp stamina to max
			if (Stamina >= MaxStamina)
			{
				Stamina = MaxStamina;
				bStaminaDrained = false;
				UpdateMovementParams();
			}
		}
	}
}