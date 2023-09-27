// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"


UENUM(BlueprintType)
enum class EITems : uint8 {
	ITEM_None       UMETA(DisplayName="N/A"),
	ITEM_Flashlight       UMETA(DisplayName="Flashlight"),
	ITEM_Stone        UMETA(DisplayName="Stone"),
	ITEM_Pistol        UMETA(DisplayName="Pistol"),
};

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPhysicalItem
{
	GENERATED_BODY()

	FPhysicalItem()
	{
	}

	FPhysicalItem(UItemData* item, uint8 amount = 1)
	{
		this->Item = item;
		this->Amount = amount;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItemData* Item = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 Amount = 0;
};


UCLASS()
class HORRORPROJECT_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	EITems Item_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Item_Texture;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	UStaticMesh * Item_StaticMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FName Item_Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	float Size;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FString Item_Desc;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	bool Unlimited = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	bool Stackable = false;
	
};
