// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BG_Skateboard.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ABG_Skateboard::ABG_Skateboard()
{
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    SetRootComponent(CollisionComponent);
}

void ABG_Skateboard::BeginPlay()
{
    Super::BeginPlay();

    if(IsValid(CollisionComponent))CollisionComponent->OnComponentHit.AddDynamic(this, &ABG_Skateboard::OnSkateboardHit);
}

void ABG_Skateboard::OnSkateboardHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && !OtherActor->IsA(ACharacter::StaticClass()))
    {
        if (AActor* PlayerCharacter = GetOwner())
        {
            //TODO
            /*
                Drop Player from Skateboard
            */
        }
    }
}