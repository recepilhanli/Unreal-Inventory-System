// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "CollectibleItem.h"
#include "PlayerInventory.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArm->SetupAttachment(Camera);
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	//FMath::RInterpTo()

	Flashlight = CreateDefaultSubobject<USpotLightComponent>("Flashlight");
	Flashlight->SetupAttachment(SpringArm);


	Inventory = CreateDefaultSubobject<UPlayerInventory>("Inventory");

	GetCharacterMovement()->MaxWalkSpeed = Character_Speed_Walking;	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled() && !HasAuthority()) SetActorLabel(TEXT("Controlling_Player"));
	else if (HasAuthority()) SetActorLabel(TEXT("Main_Player"));
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsLocallyControlled())
	{
		SetActorTickEnabled(false);
		return;
	}


	CheckForItems();
}

void APlayerCharacter::CheckForItems(bool take)
{
	if (IsLocallyControlled() == false) return;

	//float DeltaTime = GetWorld()->DeltaTimeSeconds;
	FVector CharacterPos = Camera->GetComponentLocation();
	FVector Forward = CharacterPos + Camera->GetForwardVector() * 350;
	TArray<AActor*> Ignore;
	Ignore.Add(this);
	FHitResult result;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*> ignoredactors;
	ignoredactors.Add(GetOwner());
	bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), CharacterPos, Forward, 40,
	                                                             traceObjectTypes,
	                                                             false,
	                                                             ignoredactors, EDrawDebugTrace::None, result,
	                                                             true, FLinearColor::Green, FLinearColor::Red, 1);


	if (hit)
	{
		if (result.GetActor()->ActorHasTag(FName("PhysicalItem")))
		{
			ACollectibleItem* citem = Cast<ACollectibleItem>(result.GetActor());
			if (citem != nullptr)
			{
				if (!take) PlayerLookingItemEvent(true, citem->Fitem.Item);
				else
				{
					Server_SendRPC(ERPCType::RPC_Item_Take, citem);
					Inventory->AddItemToBackpack(citem->Fitem.Item, citem->Fitem.Amount);
				}
			}
		}
		else PlayerLookingItemEvent(false, nullptr);
	}
	else PlayerLookingItemEvent(false, nullptr);
}


//RPC
void APlayerCharacter::Server_SendRPC_Implementation(ERPCType rpc, AActor* interaction)
{
	switch (rpc)
	{
	case ERPCType::RPC_Item_Take:
		{
			auto item = Cast<ACollectibleItem>(interaction);
			if (item) item->DestroyItemServer();
			break;
		}
	case ERPCType::RPC_Toggle_Flashlight:
		{
			Inventory->FlashlightEnabled = !(Inventory->FlashlightEnabled);
			if(HasAuthority()) Inventory->ToggleFlashlight();
			
			UE_LOG(LogTemp,Warning,TEXT("a flashlight's toggled"));
			break;
		}

	default: break;
	}
}

bool APlayerCharacter::Server_SendRPC_Validate(ERPCType rpc, AActor* interaction)
{
	switch (rpc)
	{
	case ERPCType::RPC_Item_Take:
		{
			if (interaction == nullptr) return false;
			break;
		}

	case ERPCType::RPC_Toggle_Flashlight:
		{
			if (interaction != nullptr) return false;
			if(Inventory == nullptr) return false;
			break;
		}
		
	default: break;
	}

	return true;
}




// Controls
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (IsLocallyControlled() == false) return;
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookX", this, &APlayerCharacter::LookX);
	PlayerInputComponent->BindAxis("LookY", this, &APlayerCharacter::LookY);

	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &APlayerCharacter::FlashlightAction);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpAction);
	PlayerInputComponent->BindAction("UsingItem", IE_Pressed, this, &APlayerCharacter::UsingItemAction);
	PlayerInputComponent->BindAction("TakeItem", IE_Pressed, this, &APlayerCharacter::TakingItemAction);

	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const bool);
	InputComponent->BindAction<FCustomInputDelegate>("Running", IE_Pressed, this, &APlayerCharacter::SetRunningAction, true);
	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const bool);
	InputComponent->BindAction<FCustomInputDelegate>("Running", IE_Released, this, &APlayerCharacter::SetRunningAction, false);



	
}

void APlayerCharacter::SetRunningAction(bool run)
{
if(run) GetCharacterMovement()->MaxWalkSpeed = Character_Speed_Running;
else GetCharacterMovement()->MaxWalkSpeed = Character_Speed_Walking;
	UE_LOG(LogTemp,Log,TEXT("Player run state: %hhd"),run);
}


void APlayerCharacter::MoveForward(float scale)
{
	AddMovementInput(GetActorForwardVector(), scale);
}

void APlayerCharacter::MoveRight(float scale)
{
	AddMovementInput(GetActorRightVector(), scale);
}

void APlayerCharacter::LookX(float scale)
{
	AddControllerYawInput(scale);
}

void APlayerCharacter::LookY(float scale)
{
	AddControllerPitchInput(-scale);


	Inventory->CameraPitchRPC(Camera->GetRelativeRotation().Pitch);
	
}

void APlayerCharacter::JumpAction()
{
	Jump();
}

void APlayerCharacter::FlashlightAction()
{
	Server_SendRPC(ERPCType::RPC_Toggle_Flashlight,nullptr);
}

void APlayerCharacter::UsingItemAction()
{
	if (Inventory->InBackpack == true) return;
	Inventory->OnUseItem(Inventory->SelectedSlot);
}

void APlayerCharacter::TakingItemAction()
{
	if (Inventory->InBackpack == true) return;
	CheckForItems(true);
}
