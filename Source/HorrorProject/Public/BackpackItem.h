// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
/**
 * 
 */
class HORRORPROJECT_API BackpackItem
{
public:
	BackpackItem();
	~BackpackItem();

	FVector2D Position;
	UItemData* item;
};
