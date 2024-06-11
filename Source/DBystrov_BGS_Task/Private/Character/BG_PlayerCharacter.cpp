// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BG_PlayerCharacter.h"

#include "InputAction.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "BG_SpringArmComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABG_PlayerCharacter::ABG_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//..............................Set components.................................//

	SpringArmComponent = CreateDefaultSubobject<UBG_SpringArmComponent>("CameraSystem");
	SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComponent->SocketOffset = FVector(0, 100, 50);
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 100));
	SpringArmComponent->bUsePawnControlRotation = false;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCamera");
	ThirdPersonCamera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;
	ThirdPersonCamera->SetActive(true);
}


void ABG_PlayerCharacter::BeginPlay()
{	
	Super::BeginPlay();

	GetCharacterMovement()->BrakingFriction = BrakingFrictionValue;
	GetCharacterMovement()->MaxWalkSpeed = GlideSpeed;
}



void ABG_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AlignToSurface();
}



void ABG_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//................Moving................//

		EnhancedInputComponent->BindAction(GetActionByName("Move"), ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(GetActionByName("EmergencyStop"), ETriggerEvent::Triggered, this, &ThisClass::EmergencyStop);

		EnhancedInputComponent->BindAction(GetActionByName("Look"), ETriggerEvent::Triggered, this, & ThisClass::Look);

		EnhancedInputComponent->BindAction(GetActionByName("Jump"), ETriggerEvent::Triggered, this, &ThisClass::OnJumpPressed);

		EnhancedInputComponent->BindAction(GetActionByName("SprintStart"), ETriggerEvent::Triggered, this, &ThisClass::OnPressSprint);
		EnhancedInputComponent->BindAction(GetActionByName("SprintEnd"), ETriggerEvent::Triggered, this, &ThisClass::OnReleaseSprint);

		ULocalPlayer* LLocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
		auto EnhancedInputSubsystem = LLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LLocalPlayer);
		if (IsValid(EnhancedInputSubsystem)) 
		{
			EnhancedInputSubsystem->AddMappingContext(Context, 0);
		}
	}

}

void ABG_PlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void ABG_PlayerCharacter::OnJumpPressed()
{
	Jump();
}


UInputAction* ABG_PlayerCharacter::GetActionByName(const FName ActionName)
{
	auto ActionLink = InputActions.Find(ActionName);
	check(ActionLink);

	return *ActionLink;
}

void ABG_PlayerCharacter::ChangeMoveType_Implementation(EMoveType NewMoveType)
{
	MoveType = NewMoveType;

	OnMoveTypeChanged(NewMoveType);
	OnMoveTypeChangedBind.Broadcast(NewMoveType);
}

void ABG_PlayerCharacter::OnPressSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	GetCharacterMovement()->BrakingFriction = GlidingFrictionValue;
	ChangeMoveType(EMoveType::Sprint);
}

void ABG_PlayerCharacter::OnReleaseSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = GlideSpeed;
	ChangeMoveType(EMoveType::Glide);
}


void ABG_PlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	FRotator LookRotate = FRotator(0, GetControlRotation().Yaw, 0);

	AddMovementInput(FRotationMatrix(LookRotate).GetUnitAxis(EAxis::X), MovementVector.Y);
}

void ABG_PlayerCharacter::EmergencyStop()
{
	GetCharacterMovement()->BrakingFriction = BrakingFrictionValue;
}

void ABG_PlayerCharacter::AlignToSurface()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		FVector SurfaceNormal = HitResult.Normal;
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromXZ(GetActorForwardVector(), SurfaceNormal);
		SetActorRotation(NewRotation);
	}
}