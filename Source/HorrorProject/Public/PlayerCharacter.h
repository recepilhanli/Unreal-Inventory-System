// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class ERPCType : uint8 {
	RPC_Item_Take,
	RPC_Toggle_Flashlight,
};


UCLASS()
class HORRORPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Movement
	void MoveForward(float scale);
	void MoveRight(float scale);
	void LookX(float scale);
	void LookY(float scale);
	void JumpAction();
	void FlashlightAction();
	void TakingItemAction();
	void UsingItemAction();
	void CheckForItems(bool take = false);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void PlayerLookingItemEvent(bool onscreen, class UItemData * item);
		
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Base")
	class UCameraComponent * Camera;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Base")
	class USpringArmComponent * SpringArm;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Inventory")
	class USpotLightComponent * Flashlight;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Inventory")
	class UPlayerInventory * Inventory;

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_SendRPC(ERPCType rpc, AActor * interaction);





	
};
