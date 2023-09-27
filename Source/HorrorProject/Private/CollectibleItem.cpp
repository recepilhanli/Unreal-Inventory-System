// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleItem.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACollectibleItem::ACollectibleItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	MeshComponent->SetupAttachment(RootComponent);


	if (Fitem.Item != nullptr)
	{
		MeshComponent->SetStaticMesh(Fitem.Item->Item_StaticMesh);
	}

	MeshComponent->SetSimulatePhysics(true);

	Tags.Add(FName("PhysicalItem"));
}


void ACollectibleItem::BeginPlay()
{
	Super::BeginPlay();
	RefreshItem();
}

void ACollectibleItem::RefreshItem_Implementation()
{
	if (Fitem.Item != nullptr)
	{
		MeshComponent->SetStaticMesh(Fitem.Item->Item_StaticMesh);
	}
}

void ACollectibleItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(ACollectibleItem, Fitem);
}


void ACollectibleItem::OnFItemRep()
{
	RefreshItem();
}


void ACollectibleItem::DestroyItemServer_Implementation()
{
	Destroy();
}

bool ACollectibleItem::DestroyItemServer_Validate()
{
	return true;
}
