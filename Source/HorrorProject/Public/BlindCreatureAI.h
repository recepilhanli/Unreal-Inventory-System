// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "BlindCreatureAI.generated.h"

UCLASS()
class HORRORPROJECT_API ABlindCreatureAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlindCreatureAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnPerception(AActor * Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void OnPerceptionForgotten(AActor* Actor);
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UAIPerceptionComponent * PerceptionComponent;
	class UAISenseConfig_Sight * sightConfig;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStaticMeshComponent * smesh;
};
