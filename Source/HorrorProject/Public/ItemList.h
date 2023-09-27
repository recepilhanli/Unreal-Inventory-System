// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataAsset.h"
#include "ItemList.generated.h"

/**
 * 
 */
UCLASS()
class HORRORPROJECT_API UItemList : public UPrimaryDataAsset
{
	
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "List")
	TArray<UItemData*> ItemArray;
	
};
