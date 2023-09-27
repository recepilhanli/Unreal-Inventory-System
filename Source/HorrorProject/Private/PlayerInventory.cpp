// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

#include "PlayerCharacter.h"
#include "ActorList.h"
#include "CollectibleItem.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Net/UnrealNetwork.h"


UItemList* UPlayerInventory::ItemList = nullptr;

UPlayerInventory::UPlayerInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
	
	if (ItemList == nullptr)
	{
		ItemList = LoadObject<UItemList>(nullptr,TEXT("/Game/Misc/Items/List/ItemList"));
	}

	OwnedCharacter = Cast<APlayerCharacter>(GetOwner());
}


void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();

	AddItemToPocket(GetItem(EITems::ITEM_Flashlight));
	AddItemToPocket(GetItem(EITems::ITEM_Stone), 5);
	AddItemToPocket(GetItem(EITems::ITEM_None));

	AddItemToBackpack(GetItem(EITems::ITEM_Flashlight));
	AddItemToBackpack(GetItem(EITems::ITEM_Stone), 25);

	ToggleFlashlight();
}



void UPlayerInventory::CameraPitchRPC_Implementation(float value)
{
	CameraPitch = value;
	if(OwnedCharacter->HasAuthority()) SetCameraPitch();
}

void UPlayerInventory::SetCameraPitch()
{
			
	FRotator rot = OwnedCharacter->Camera->GetRelativeRotation();
	
	OwnedCharacter->Camera->SetRelativeRotation(FRotator(CameraPitch,rot.Yaw,rot.Roll));
}


bool UPlayerInventory::CameraPitchRPC_Validate(float value)
{
	if(OwnedCharacter == nullptr) return false;
	
	return true;
}

void UPlayerInventory::On_Rep_CameraPitch()
{
	if(OwnedCharacter->HasAuthority()) return;
	if(OwnedCharacter->IsLocallyControlled() == false) SetCameraPitch();
}




void UPlayerInventory::ExChangeItem(uint8 backpackindex, uint8 pocketindex, bool addtobackpack)
{
	if (addtobackpack == false)
	{
		FPhysicalItem bfitem = BackpackItems[backpackindex];
		FPhysicalItem pfitem = PocketItems[pocketindex];

		PocketItems[pocketindex] = bfitem;

		if (pfitem.Item != GetItem(EITems::ITEM_None)) BackpackItems[backpackindex] = pfitem;
		else BackpackItems.RemoveAt(backpackindex);
	}
	else
	{
		FPhysicalItem pfitem = PocketItems[pocketindex];
		if (pfitem.Item == GetItem(EITems::ITEM_None)) return;
		BackpackItems.Add(pfitem);
		PocketItems[pocketindex].Item = GetItem(EITems::ITEM_None);
		PocketItems[pocketindex].Amount = 0;
	}
}


void UPlayerInventory::AddItemToBackpack(UItemData* item, uint8 amount)
{
	if (item->Stackable == false)
	{
		BackpackItems.Add(FPhysicalItem(item, amount));
	}
	else
	{
		for (int i = 0; i < BackpackItems.Num(); i++)
		{
			if (BackpackItems[i].Item == item)
			{
				BackpackItems[i].Amount += amount;
				return;
			}
		}

		BackpackItems.Add(FPhysicalItem(item, amount));
	}
}

void UPlayerInventory::AddItemToPocket(UItemData* item, uint8 amount)
{
	PocketItems.Add(FPhysicalItem(item, amount));
}


void UPlayerInventory::OnUseItem(uint8 slotid)
{
	UItemData* item = PocketItems[slotid].Item;

	if (item == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,TEXT("NULL returned."));
		return;
	}


	switch (item->Item_ID)
	{
	case EITems::ITEM_Flashlight:
		{
			OwnedCharacter->FlashlightAction();
			break;
		}
	case EITems::ITEM_Stone:
		{
			UActorList* List = LoadObject<UActorList>(nullptr,TEXT("/Game/Blueprints/List/ActorList"));
			if (List)
			{
				FVector SpawnLocation = OwnedCharacter->GetActorLocation() + OwnedCharacter->Camera->GetForwardVector()
					* 100 + OwnedCharacter->Camera->GetRelativeLocation();
				FRotator SpawnRotation = OwnedCharacter->Camera->GetComponentRotation();
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(List->Actors[Actor_Stone], SpawnLocation,
				                                                      SpawnRotation);
			}
			break;
		}

	default: break;
	}

	if (item->Unlimited == false)
	{
		PocketItems[slotid].Amount --;
		if (PocketItems[slotid].Amount <= 0)
		{
			PocketItems.RemoveAt(slotid);
			FPhysicalItem _item = FPhysicalItem(GetItem(EITems::ITEM_None));
			PocketItems.Insert(_item, slotid);
		}
	}
}



UItemData* UPlayerInventory::GetItem(EITems item)
{
	return ItemList->ItemArray[(uint8)item];
}

void UPlayerInventory::DropItem_Server_Implementation(FVector Pos, UItemData* item, uint8 amount)
{
	UE_LOG(LogTemp, Warning, TEXT("an item dropped."));

	FPhysicalItem fitem;
	fitem.Item = item;
	fitem.Amount = amount;

	ACollectibleItem* spawned = GetWorld()->SpawnActor<ACollectibleItem>(
		ACollectibleItem::StaticClass(), Pos, FRotator(0, 0, 0));

	if (spawned)
	{
		spawned->Fitem = fitem;
		spawned->RefreshItem();
	}
}

bool UPlayerInventory::DropItem_Server_Validate(FVector Pos, UItemData* item, uint8 amount)
{
	return true;
}


void UPlayerInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerInventory, CameraPitch);
	DOREPLIFETIME(UPlayerInventory, FlashlightEnabled);
}

void UPlayerInventory::OnFlashlightToggled_Rep()
{
	if(OwnedCharacter->HasAuthority()) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Replicated. %hhd"),FlashlightEnabled);

	ToggleFlashlight();
}

void UPlayerInventory::ToggleFlashlight()
{
	if (FlashlightEnabled)
	{
		OwnedCharacter->Flashlight->SetActive(true);
		OwnedCharacter->Flashlight->SetVisibility(true);
	}
	else
	{
		OwnedCharacter->Flashlight->SetActive(false);
		OwnedCharacter->Flashlight->SetVisibility(false);
	}
}





// // Called every frame
// void UPlayerInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }
