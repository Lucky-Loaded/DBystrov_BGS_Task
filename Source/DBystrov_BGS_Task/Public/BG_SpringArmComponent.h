// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "BG_SpringArmComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Camera, meta = (BlueprintSpawnableComponent), hideCategories = (Mobility))
class DBYSTROV_BGS_TASK_API UBG_SpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

	//constructor
public:

	UBG_SpringArmComponent();





//c++ protected methods
protected:


	virtual void OnRegister() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateCameraDistance(float DeltaTime);


	void CameraModifyByPitch();


//c++ private methods
private:







//c++ private values
private:

	AActor* MyActor = nullptr;


	float NewTargetArmLength;
	float SavedCameraDistance = 0.0f;
	bool IsCameraDistanceLocked = false;
	bool IsCameraDistanceChanged = false;

	float SpawnFocalSpeed = 0.0f;
	float SpawnCameraDistance;


protected:

	class ABG_PlayerCharacter* PlayerCharacter = nullptr;

	bool IsPlayerFastMoving = false;
	bool IsPlayerFastMovingCache = false;

//>>................................................................................................................................................................................<<//


//Blueprint methods
public:

	//.....................................CameraDistance..........................................//
	/*
		Enable CameraDistance changing and interpolate current value to value before locking.
	*/
	UFUNCTION(BlueprintCallable, Category = "CameraSystem|CameraDistance")
	void ResetCameraDistance();
	/*
		Set new value of CameraDistance to interpolation.
	*/
	UFUNCTION(BlueprintCallable, Category = "CameraSystem|CameraDistance")
	void SetNewCameraDistance(float NewDistance);

	/*
		Set new value of CameraDistance to interpolation and
		Save current TargetArmLength so it can be reverted
	*/
	UFUNCTION(BlueprintCallable, Category = "CameraSystem|CameraDistance")
	void SetNewCameraDistanceAndSavePrevious(float NewDistance);
	/*
		@return IsCameraDistanceChanged.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CameraSystem|CameraDistance")
	bool GetIsCameraDistanceChanged() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraSystem|CameraSettings")
		void OnPlayerIsFastMovingChanged();
		virtual void OnPlayerIsFastMovingChanged_Implementation();


//Blueprint values
public:

	//..........................................Focal..............................................//

	/*
		Speed of TargetArmLength change interpolation.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "CameraSettings|Focal")
	float FocalSpeed = 3.0f;
	/*
		Multiplier for zooming in ChangeCameraDistance.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "CameraSettings|Focal")
	float FocalStepSpeed = 100.0f;
	/*
		Camera distance for sprint move type.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "PlayerCameraSystem|CameraSettings")
	float SprintCameraDistance = 300.0f;
	/*
		Max spring arm length.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "CameraSettings|Focal")
	float MaxCameraDistance = 500.0f;
	/*
		Min spring arm length.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "CameraSettings|Focal")
	float MinCameraDistance = 110.0f;

	/*
		 Arm length modifer on upper camera angle.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "CameraSettings|Focal")
	float ModifyCameraArmLenght = 50.0f;
	/*
		Starting upper view length interpolation from camera pitch angle (e.g. 360° is camera middle position)
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "360.0", UIMax = "360.0"), Category = "CameraSettings|SocketOffset")
	float StartLengthModifyInterpolationAngle = 360.0f;
};
