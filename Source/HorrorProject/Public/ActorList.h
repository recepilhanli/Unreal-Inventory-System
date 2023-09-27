// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActorList.generated.h"

#define		Actor_PhysicalItem		(0)
#define		Actor_Stone				(1)



/**
 * 
 */
UCLASS()
class HORRORPROJECT_API UActorList : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "List")
	TArray< TSubclassOf<AActor> > Actors;
};
