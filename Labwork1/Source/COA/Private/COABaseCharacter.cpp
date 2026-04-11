 // Fill out your copyright notice in the Description page of Project Settings.


#include "COABaseCharacter.h"

// Sets default values
ACOABaseCharacter::ACOABaseCharacter():

	bDead(false),
	Health(100.f),
	MaxHealth(100.f),
	HealingRate(0.0f),
	WalkSpeed(300.f)
{		
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called when the game starts or when spawned
void ACOABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACOABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDead) {
		Health = FMath::Min(MaxHealth,Health + HealingRate * DeltaTime);
	}
}


