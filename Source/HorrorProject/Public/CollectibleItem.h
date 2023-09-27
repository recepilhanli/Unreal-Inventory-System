// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "GameFramework/Actor.h"
#include "CollectibleItem.generated.h"

UCLASS()
class HORRORPROJECT_API ACollectibleItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACollectibleItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = OnFItemRep)
	FPhysicalItem Fitem;


	UFUNCTION()
	void OnFItemRep();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void DestroyItemServer();

	UFUNCTION(NetMulticast, Reliable)
	void RefreshItem();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
