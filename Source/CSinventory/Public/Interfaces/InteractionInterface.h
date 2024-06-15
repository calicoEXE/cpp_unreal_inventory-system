// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ACSinventoryCharacter;

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	Use UMETA(DisplayName = "Use"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_BODY()

	FInteractableData() : InteractableType(EInteractableType::Pickup), // when constructor runs, it will run eg. Pickup first
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	InteractionDuration((0.0f))
	{
	}

	
	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	UPROPERTY(EditInstanceOnly)
	int8 Quantity;
	// use for pickups

	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
	// for using items/eating
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class CSINVENTORY_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(ACSinventoryCharacter* PlayerCharacter);
	// -virtual- function that can be overwritten by child classes

	FInteractableData InteractableData;
	
	
};
