// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindCreatureAI.h"

#include "PlayerCharacter.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
// Sets default values
ABlindCreatureAI::ABlindCreatureAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	smesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static_Mesh"));
	
	smesh->SetupAttachment(RootComponent);
	
	sightConfig->SightRadius = 2000.0f;
	sightConfig->LoseSightRadius = sightConfig->SightRadius + 500.0f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABlindCreatureAI::ABlindCreatureAI::OnPerception);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ABlindCreatureAI::ABlindCreatureAI::OnPerceptionForgotten);
	
}


void ABlindCreatureAI::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	APlayerCharacter * character = Cast<APlayerCharacter>(Actor);
	if(character)
	{
		
		UE_LOG(LogTemp,Warning,TEXT("Perception %d"),Stimulus.Type.Index);
	}
}

void ABlindCreatureAI::OnPerceptionForgotten(AActor* Actor)
{
	APlayerCharacter * character = Cast<APlayerCharacter>(Actor);
	if(character)
	{
		UE_LOG(LogTemp,Warning,TEXT("Perception forgotten"));
	}
}



// Called when the game starts or when spawned
void ABlindCreatureAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlindCreatureAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABlindCreatureAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

