// Fill out your copyright notice in the Description page of Project Settings.


#include "BG_SpringArmComponent.h"

#include "Character/BG_PlayerCharacter.h"






UBG_SpringArmComponent::UBG_SpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PlayerCharacter = Cast<ABG_PlayerCharacter>(GetOwner());
}




void UBG_SpringArmComponent::OnRegister()
{
	Super::OnRegister();

	SpawnFocalSpeed = FocalSpeed;

	NewTargetArmLength = TargetArmLength;
	SpawnCameraDistance = TargetArmLength;
	SavedCameraDistance = TargetArmLength;

}





void UBG_SpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(PlayerCharacter) || PlayerCharacter->GlideSpeed == 0.0f) return;

	auto LVelocity = PlayerCharacter->GetVelocity();
	IsPlayerFastMoving = PlayerCharacter->GetVelocity().Length() > PlayerCharacter->GlideSpeed;

	if(IsPlayerFastMovingCache != IsPlayerFastMoving)
	{
		OnPlayerIsFastMovingChanged();
		IsPlayerFastMovingCache = IsPlayerFastMoving;
	}
	UpdateCameraDistance(DeltaTime);
}


void UBG_SpringArmComponent::UpdateCameraDistance(float DeltaTime)
{
	TargetArmLength = FMath::FInterpTo(TargetArmLength, NewTargetArmLength, DeltaTime, FocalSpeed);
}

void UBG_SpringArmComponent::ResetCameraDistance()
{
	IsCameraDistanceLocked = false;
	IsCameraDistanceChanged = false;
	NewTargetArmLength = SavedCameraDistance;
}


void UBG_SpringArmComponent::SetNewCameraDistance(float NewDistance)
{
	if (!IsCameraDistanceLocked)
	{
		NewTargetArmLength = FMath::Clamp(NewDistance, MinCameraDistance, MaxCameraDistance);
	}
}

void UBG_SpringArmComponent::SetNewCameraDistanceAndSavePrevious(float NewDistance)
{
	SavedCameraDistance = TargetArmLength;
	

	IsCameraDistanceChanged = true;
	SetNewCameraDistance(NewDistance);
}

bool UBG_SpringArmComponent::GetIsCameraDistanceChanged() const
{
	return IsCameraDistanceChanged;
}

void UBG_SpringArmComponent::OnPlayerIsFastMovingChanged_Implementation()
{
	if (!IsPlayerFastMoving && GetIsCameraDistanceChanged())
	{
		ResetCameraDistance();
	}
	else if (IsPlayerFastMoving && PlayerCharacter->GetMoveType() == EMoveType::Sprint)
	{
		SetNewCameraDistanceAndSavePrevious(SprintCameraDistance);
	}
}