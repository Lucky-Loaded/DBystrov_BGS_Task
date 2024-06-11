// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "BG_PlayerCharacter.generated.h"



UENUM(BlueprintType)
enum class EMoveType : uint8
{
	Glide		UMETA(DisplayName = "Glide"),
	Sprint		UMETA(DisplayName = "Sprint")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveTypeChanged, EMoveType, NewMoveType);


UCLASS()
class DBYSTROV_BGS_TASK_API ABG_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABG_PlayerCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Look(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);
	void EmergencyStop();
	void OnJumpPressed();
	void OnPressSprint();
	void OnReleaseSprint();


	void AlignToSurface();


//c++ private values
private:

	UPROPERTY()
	EMoveType MoveType;

//c++ public values
private:



//Blueprint methods
public:


	UFUNCTION(BlueprintCallable, BlueprintPure)
		EMoveType GetMoveType() { return MoveType; };

	UFUNCTION(BlueprintCallable)
		UInputAction* GetActionByName(const FName ActionName);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "BaseCharacter|Moving")
		void ChangeMoveType(EMoveType NewLegMoveType);
		void ChangeMoveType_Implementation(EMoveType NewLegMoveType);
	UPROPERTY(BlueprintAssignable, Category = "BaseCharacter|EventsForBind")
		FOnMoveTypeChanged OnMoveTypeChangedBind;


	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected), Category = "BaseCharacter")
		void OnMoveTypeChanged(EMoveType NewLegMoveTyp);
		virtual void OnMoveTypeChanged_Implementation(EMoveType NewLegMoveTyp) { }

//Bluepring values
public:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter|Components")
		class UBG_SpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter|Components")
		class UCameraComponent* ThirdPersonCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TMap<FName, UInputAction*> InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "PlayerCharacter")
		float GlideSpeed = 400.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "PlayerCharacter")
		float SprintSpeed = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "PlayerCharacter")
		float GlidingFrictionValue = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "PlayerCharacter")
		float BrakingFrictionValue = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputMappingContext* Context = nullptr;
};
