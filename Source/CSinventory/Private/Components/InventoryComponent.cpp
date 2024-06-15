// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Actions/PawnAction.h"
#include "Items/ItemBase.h"
#include "Data/ItemDataStructs.h"
#include "UserInterface/Inventory/InventoryHoverInfo.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemInInventory) const
{
	if (ItemInInventory)
	{
		if (InventoryContents.Contains(ItemInInventory))
		{
			return ItemInInventory;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindItemByID(UItemBase* ItemInInventory) const
{
	if (ItemInInventory)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemInInventory))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemInInventory) const
{
	// used predicate to check if condition (InventoryContents) is satisfactory (some unreal fancy shiz??)
	// used lambda to be separately checked via InventoryContents and those considered will be assigned as an
	// "Inventory Item", before checking conditions met after
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemInInventory](const UItemBase* InventoryItem)
		{
			// checks if the ID of InventoryItem matches the ID of existing item in inventory and if item is fully stacked
			return InventoryItem->ID == ItemInInventory->ID && !InventoryItem->GetMaxItemStackSize();
		}
		))
	{
		return *Result;
		// searches through inventory contents array to find item that has the same ID as one in inventory AND it's not fully stacked
		// if found, assign it as result
	}
		return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemInInventory, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetStackCapacity() - InventoryTotalWeight) / ItemInInventory->GetItemSingleWeight()); // rounding down to nearest integer
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
	
}

int32 UInventoryComponent::CalculateAmountForFullStack(UItemBase* ExistingStackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountForFullStack = ExistingStackableItem->ItemNumberData.MaxStackSize - ExistingStackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountForFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemInInventory, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemInInventory->Quantity);

	ItemInInventory->SetQuantity(ItemInInventory->Quantity - ActualAmountToRemove);

	//InventoryTotalWeight -= ActualAmountToRemove + ItemInInventory->GetItemSingleWeight();
	InventoryTotalWeight -= ActualAmountToRemove * ItemInInventory->GetItemSingleWeight();
	
	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
	
}

void UInventoryComponent::SplittingStack(UItemBase* ItemInInventory, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity)) // check if adding 1 more item doesn't overflow the inventory's capacity
	{
		RemoveAmountOfItem(ItemInInventory, AmountToSplit); // if there's enough space in inventory, split x amount from existing item
		AddNewItem(ItemInInventory, AmountToSplit); // and add it back in as a new stack
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* ItemInput)
{
	// check if input item has valid weight
	if (FMath::IsNearlyZero(ItemInput->GetItemSingleWeight()) || ItemInput->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("{0}: Item has invalid weight value."), ItemInput->ItemTextData.Name));
	}

	// check if item weight will overflow weight capacity
	if (InventoryTotalWeight + ItemInput->GetItemSingleWeight() > GetStackCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("{0}: Item would overflow stack limit."), ItemInput->ItemTextData.Name));
	}

	// slot would be full if added more
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("{0}: Inventory slots is full."), ItemInput->ItemTextData.Name));
	}

	AddNewItem(ItemInput, 1);
	return FItemAddResult::AddedAll(1, FText::Format
		(FText::FromString("Added one {0} to inventory."), ItemInput->ItemTextData.Name));
		
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemInInventory, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemInInventory->GetMaxItemStackWeight()))
	{
		return 0;
	}

	int32 DistributionAmount = RequestedAddAmount;

	// check to see if item player wants to pick up already exist inside inventory
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemInInventory); 

	// to distribute item stack over existing stacks
	while (ExistingItemStack)
	{
		// check how many items needed to make the next full stack
		const int32 AmountToMakeFullStack = CalculateAmountForFullStack(ExistingItemStack, DistributionAmount);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// when item doesnt overflow weight capacity
		if (WeightLimitAddAmount > 0)
		{
			// adjust for when it goes into inventory
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// subtracting stack on floor against inventory's limit
			DistributionAmount -= WeightLimitAddAmount;

			// left on ground
			ItemInInventory->SetQuantity(DistributionAmount);

			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				UE_LOG(LogTemp, Warning, TEXT("Inventory is full! Unable to add more. Although there are still some left on the floor..."));
				// player's initial request to add amount - ones left on floor due to insufficient space
				return RequestedAddAmount - DistributionAmount;

			}
		} 
		else if (WeightLimitAddAmount <= 0)
		{
			// for when distribution across multiple stacks, and inventory weight is maxxed out
			if (DistributionAmount != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				UE_LOG(LogTemp, Warning, TEXT("Distribution failed. Inventory is too heavy! Unable to add more."));
				return RequestedAddAmount - DistributionAmount;
			}

			return 0;
		}

		// distributed everything
		if (DistributionAmount <= 0)
		{
			OnInventoryUpdated.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("Item has all been added to inventory."));
			return RequestedAddAmount;
		}
		// check if there's any other partial stack of the item in inventory
		ExistingItemStack = FindNextPartialStack(ItemInInventory);
	}

	// when there's no more partial stacks found in inventory
	// check if there's inventory slots capacity available for a new stack
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemInInventory, DistributionAmount);

		if (WeightLimitAddAmount > 0)
		{
			if (WeightLimitAddAmount < DistributionAmount)
			{
				// if there's still some amount on floor that inventory weight does not have the capacity to handle
				DistributionAmount -= WeightLimitAddAmount;
				ItemInInventory->SetQuantity(DistributionAmount);

				AddNewItem(ItemInInventory->CreateItemCopy(), WeightLimitAddAmount); // create a copy stack on ground
				UE_LOG(LogTemp, Warning, TEXT("Inventory is too heavy! Unable to add more."));
				return RequestedAddAmount - DistributionAmount;
			}
			// if all item on floor has been picked up (and inventory has the stack space and capacity to handle)
			AddNewItem(ItemInInventory, DistributionAmount);
			UE_LOG(LogTemp, Warning, TEXT("Item has all been added to inventory"));
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - DistributionAmount;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* ItemInput)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = ItemInput->Quantity;

		// to handle non stackable items
		if (!ItemInput->ItemNumberData.IsItemStackable)
		{
			return HandleNonStackableItems(ItemInput);
		}

		// to handle stackable items
		const int32 StackableAmountAdded = HandleStackableItems(ItemInput, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format
			(FText::FromString("Added {0} to the inventory."),
			InitialRequestedAddAmount,
			ItemInput->ItemTextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format
			(FText::FromString("Partial amount of {0} added to the inventory. Amount added: {1}"),
			ItemInput->ItemTextData.Name,
			StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("Unable to add {0} to the inventory. Inventory is either full or invalid amount."),
			ItemInput->ItemTextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("GetOwner() check failed."));
	
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;
	// check if item is either a copy (into inventory) or a pick up (from world, before destroy)
	if (Item->bIsItemCopy || Item->bIsItemPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		NewItem = Item->CreateItemCopy(); // for split
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetMaxItemStackWeight();
	OnInventoryUpdated.Broadcast();
	
}

void UInventoryComponent::UseItem(UItemBase* ItemUsed)
{
	if (ItemUsed->ItemType != EItemType::Weapon)
	{
		if (ItemUsed->GetItemSingleWeight())
		{
			if (ItemUsed->Quantity > 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("ONE Item has been used from stack."));
				RemoveAmountOfItem(ItemUsed, 1);
			}
			else if (ItemUsed->Quantity == 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("ONE Item has been used (either from a non stackable or left with 1 from stack)."));
				RemoveAmountOfItem(ItemUsed, 1);
			}
		}
	}
	else
	{
		UseWeapon(ItemUsed);
	}
}

void UInventoryComponent::UseWeapon(UItemBase* ItemUsed)
{
	if (ItemUsed->bIsWeaponInUse)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is not in use."));
		ItemUsed->bIsWeaponInUse = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is being used."));
		ItemUsed->bIsWeaponInUse = true;
	}
}