// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainInvMenu.h"

#include "Items/ItemBase.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"
#include "World/Pickup.h"
#include "Player/CSinventoryCharacter.h"

void UMainInvMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainInvMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ACSinventoryCharacter>(GetOwningPlayerPawn());
}

bool UMainInvMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) // checks the conditions to initiate the drag and drop
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	 
	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		// passing dragged item and quantity to player
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}
