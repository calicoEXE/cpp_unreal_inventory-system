// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ItemBase.h"
#include "Interfaces/InteractionInterface.h"
#include "Player/CSinventoryCharacter.h"
#include "Pickup.generated.h"

class UDataTable;
class UItemBase;

UCLASS()
class CSINVENTORY_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	// PROPERTIES & VARIABLES ===============================================================

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	// FUNCTIONS ============================================================================
	
	APickup();

	void InitializedPickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity); // pull from database when beginplay

	void InitializedDrop(UItemBase* ItemToDrop, const int32 InQuantity); // pull from existing item reference

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;


protected:
	// PROPERTIES & VARIABLES ===============================================================
	
	// FUNCTIONS ============================================================================
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Interact(ACSinventoryCharacter* PlayerCharacter) override;
	void UpdateInteractableData();
	
	void TakePickup(const ACSinventoryCharacter* PlayerTake);
};
