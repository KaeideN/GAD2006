#include "ANetAvatar.h"
#include "ANetGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ANetAvatar::ANetAvatar()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	bIsRunning = false;
}

void ANetAvatar::BeginPlay()
{
	Super::BeginPlay();

	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	if (WalkSpeed <= 0.f)
	{
		WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}
	UpdateMovementSpeed();
}

void ANetAvatar::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority())
	{
		return;
	}

	ANetAvatar* OtherAvatar = Cast<ANetAvatar>(OtherActor);
	if (!OtherAvatar)
	{
		return;
	}

	if (ANetGameMode* GameMode = GetWorld()->GetAuthGameMode<ANetGameMode>())
	{
		GameMode->AvatarsOverlapped(this, OtherAvatar);
	}
}

void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &ANetAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetAvatar::MoveRight);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ANetAvatar::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ANetAvatar::StopRun);
}

void ANetAvatar::MoveForward(float Scale)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Scale);
	}
}

void ANetAvatar::MoveRight(float Scale)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Scale);
	}
}

void ANetAvatar::StartRun()
{
	ServerSetRunning(true);
}

void ANetAvatar::StopRun()
{
	ServerSetRunning(false);
}

void ANetAvatar::ServerSetRunning_Implementation(bool bNewRunning)
{
	bIsRunning = bNewRunning;

	if (HasAuthority())
	{
		OnRep_IsRunning();
	}
}

void ANetAvatar::OnRep_IsRunning()
{
	UpdateMovementSpeed();
}

void ANetAvatar::UpdateMovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunSpeed : WalkSpeed;
}

void ANetAvatar::Defeat_Implementation()
{
}

void ANetAvatar::Victory_Implementation()
{
}

void ANetAvatar::OnPlayerLost()
{
	Defeat();
}

void ANetAvatar::OnPlayerWon()
{
	Victory();
}

void ANetAvatar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetAvatar, bIsRunning);
}
