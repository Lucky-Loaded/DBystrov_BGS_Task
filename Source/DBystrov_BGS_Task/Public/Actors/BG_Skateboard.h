// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BG_Skateboard.generated.h"

UCLASS()
class DBYSTROV_BGS_TASK_API ABG_Skateboard : public AActor
{
	GENERATED_BODY()
	
public:	


	ABG_Skateboard();


protected:


	virtual void BeginPlay() override;

public:


//C++ methods
private:

	UFUNCTION()
	void OnSkateboardHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

//C++ values
private:

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* CollisionComponent;
};
