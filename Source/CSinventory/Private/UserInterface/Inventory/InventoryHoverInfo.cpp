// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryHoverInfo.h"

#include "Components/TextBlock.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"


void UInventoryHoverInfo::NativeConstruct()
{
	Super::NativeConstruct();

	UItemBase* ItemBeingHovered = HoveringOverInventorySlot->GetItemReference();

	switch(ItemBeingHovered->ItemType)
	{
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable Item"));
		break;
	case EItemType::Weapon:
		ItemType->SetText(FText::FromString("Weapon Item"));
		break;
	case EItemType::Resource:
		ItemType->SetText(FText::FromString("Resource Item"));
		break;
	default: ;
	}
	
	ItemName->SetText(ItemBeingHovered->ItemTextData.Name);
	ItemDescription->SetText(ItemBeingHovered->ItemTextData.Description);
	
	FText UsabilityText = FText::FromString(ItemBeingHovered->ItemUsabilityCheck.ItemUsable ? "Usable" : "Not Usable");
	ItemUsabilityCheck->SetText(UsabilityText);

	// stackability
	StackSizeColumn->SetText(FText::FromString("Stack Size"));
	ItemSingleStackSizeValue->SetText(FText::AsNumber(ItemBeingHovered->Quantity));
	Slash->SetText(FText::FromString("/"));
	MaxItemStackSize->SetText(FText::AsNumber(ItemBeingHovered->ItemNumberData.MaxStackSize));
	ItemStackability->SetText(FText::FromString("Item is not stackable!")); // don't show this line if item is stackable
	
	ItemSingleStackWeightValue->SetText(FText::AsNumber(ItemBeingHovered->GetItemSingleWeight())); // single item's stack weight
	MaxItemStackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetMaxItemStackWeight())); // maximum item's stack weight
	
	if(ItemBeingHovered->ItemNumberData.IsItemStackable)
	{
		MaxItemStackSize->SetText(FText::AsNumber(ItemBeingHovered->ItemNumberData.MaxStackSize));
		ItemStackability->SetVisibility(ESlateVisibility::Collapsed); // don't show this line if item is stackable
	}
	else
	{
		//StackSizeColumn->SetVisibility(ESlateVisibility::Collapsed);
		ItemSingleStackSizeValue->SetVisibility(ESlateVisibility::Collapsed);
		Slash->SetVisibility(ESlateVisibility::Collapsed);
		MaxItemStackSize->SetVisibility(ESlateVisibility::Collapsed);
		ItemStackability->SetVisibility(ESlateVisibility::Visible); // show this if item is NOT stackable
	}
	
}
