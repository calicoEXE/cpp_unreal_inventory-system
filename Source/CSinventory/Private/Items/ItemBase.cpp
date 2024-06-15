// Fill out your copyright notice in the Description page of Project Settings.


#include "CSInventory/Public/Items/ItemBase.h"
#include "CSInventory/Public/Player/CSinventoryCharacter.h"
#include "Components/InventoryComponent.h"

UItemBase::UItemBase() : bIsItemCopy(false), bIsItemPickup(false)
{
	
}

void UItemBase::ResetItemFlags()
{
	bIsItemCopy = false;
	bIsItemPickup = false;
}

UItemBase* UItemBase::CreateItemCopy()
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemTextData = this->ItemTextData;
	ItemCopy->ItemNumberData = this->ItemNumberData;
	ItemCopy->ItemUsabilityCheck = this->ItemUsabilityCheck;
	ItemCopy->ItemAssetData = this->ItemAssetData;
	ItemCopy->bIsItemCopy = true;

	return  ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		if (NewQuantity <= 0)
		{
			Quantity = 0;
		}
		else
		{
			if (ItemNumberData.IsItemStackable)
			{
				Quantity = FMath::Clamp(NewQuantity, 0, ItemNumberData.MaxStackSize);
			}
			else
			{
				Quantity = FMath::Clamp(NewQuantity, 0, 1);
			}
		}
		
		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory -> RemoveSingleInstanceOfItem(this);
			}
		}
		
	}
}

void UItemBase::Use(ACSinventoryCharacter* Character)
{
}