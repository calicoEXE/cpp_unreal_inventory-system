// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"

#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Data/ItemDataStructs.h"
#include "Player/CSinventoryCharacter.h"


APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);

}


void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializedPickup(UItemBase::StaticClass(), ItemQuantity);
}

void APickup::InitializedPickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemNumberData = ItemData->ItemNumberData;
		ItemReference->ItemUsabilityCheck = ItemData->ItemUsabilityCheck;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;
		
		if (InQuantity <= 0)
		{
			ItemReference->SetQuantity(1);
		}
		else
		{
			ItemReference->SetQuantity(InQuantity);
		}

		PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializedDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;

	if (InQuantity <= 0)
	{
		ItemReference->SetQuantity(1);
	}
	else
	{
		ItemReference->SetQuantity(InQuantity);
	}

	ItemReference->ItemNumberData.Weight = ItemToDrop->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(ItemToDrop->ItemAssetData.Mesh);
	
	UpdateInteractableData();
	
}


void APickup::UpdateInteractableData()
{
	
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->ItemTextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
	
}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(ACSinventoryCharacter* PlayerCharacter)
{
	if(PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickup::TakePickup(const ACSinventoryCharacter* PlayerTake)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = PlayerTake->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				// diff item operation depending on how many items has been added
				switch (AddResult.ItemOperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialItemAdded:
					UpdateInteractableData();
					PlayerTake->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Big error :( Inventory component is null."));
			}
		}
	}
}

