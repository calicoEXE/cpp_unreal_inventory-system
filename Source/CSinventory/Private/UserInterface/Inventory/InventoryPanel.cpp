// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInterface/Inventory/InventoryPanel.h"
#include "Player/CSinventoryCharacter.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Components/InventoryComponent.h"


#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Items/ItemBase.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACSinventoryCharacter>(GetOwningPlayerPawn());

	if(PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();

		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

void UInventoryPanel::SetInfoText() const
{

	// getting stack capacity per item
	InventoryWeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
												InventoryReference->GetInventoryTotalWeight(),
												InventoryReference->GetStackCapacity()));

	// getting slots capacity for inventory
	InventoryCapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
												InventoryReference->GetInventoryContents().Num(),
												InventoryReference->GetInventorySlotsCapacity()));
}


void UInventoryPanel::RefreshInventory()
{
	if(InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren(); // to clear off previous iteration of inventory (having newly updated inventory each time)

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}
	
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
