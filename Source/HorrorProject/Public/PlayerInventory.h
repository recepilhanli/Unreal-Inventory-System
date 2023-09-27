// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemList.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "PlayerInventory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HORRORPROJECT_API UPlayerInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInventory();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pocket")
	TArray<FPhysicalItem> PocketItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pocket")
	uint8 SelectedSlot = 0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Backpack")
	TArray<FPhysicalItem> BackpackItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	bool InBackpack = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	bool DraggingItem = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	uint8 DraggingIndex = 0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	UUserWidget * BackpackWidget = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	UUserWidget * DragWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	UUserWidget * CollectibleWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	UUserWidget * PocketWidget;
	
	class UStaticMeshComponent* ShowingMesh;
	class APlayerCharacter* OwnedCharacter;
	static class UItemList* ItemList;


	void OnUseItem(uint8 slotid);



	UFUNCTION(BlueprintCallable)
	void ExChangeItem(uint8 backpackindex,uint8 pocketindex, bool addtobackpack = false);
	
	UFUNCTION(BlueprintCallable)
	UItemData* GetItem(EITems item);
	UFUNCTION(BlueprintCallable)
	void AddItemToPocket(UItemData* item, uint8 amount = 1);
	UFUNCTION(BlueprintCallable)
	void AddItemToBackpack(UItemData* item, uint8 amount = 1);


	UFUNCTION(Server,Reliable,WithValidation,BlueprintCallable)
	void DropItem_Server(FVector pos,UItemData* item, uint8 amount = 1);


	UPROPERTY(ReplicatedUsing = OnFlashlightToggled_Rep)
	bool FlashlightEnabled = false;
	
	UFUNCTION()
	void OnFlashlightToggled_Rep();
	void ToggleFlashlight();


	UPROPERTY(ReplicatedUsing = On_Rep_CameraPitch)
	float CameraPitch;
	
	UFUNCTION(Server,Reliable,WithValidation)
	void CameraPitchRPC(float value);
	
	void SetCameraPitch();

	UFUNCTION()
	void On_Rep_CameraPitch();

	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
