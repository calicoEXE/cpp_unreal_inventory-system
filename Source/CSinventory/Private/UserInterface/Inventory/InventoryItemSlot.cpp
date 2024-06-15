// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryItemSlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/DragItemVisual.h"
#include "UserInterface/Inventory/InventoryHoverInfo.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

class UItemBase;

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (HoverInfoClass)
	{
		UInventoryHoverInfo* HoverInfo = CreateWidget<UInventoryHoverInfo>(this, HoverInfoClass);
		HoverInfo->HoveringOverInventorySlot = this;
		SetToolTip(HoverInfo);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
	 if (ItemReference)
	 {
 		switch (ItemReference->ItemType)
 		{
 		case EItemType::Consumable:
 			ItemBorder->SetBrushColor(FLinearColor::Green);
 			break;
 		case EItemType::Weapon:
 			ItemBorder->SetBrushColor(FLinearColor::Red);
 			break;
 		case EItemType::Resource:
 			ItemBorder->SetBrushColor(FLinearColor::Blue);
 			break;
 		default: ;
 		}
	 }

	ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);

	if (ItemReference->ItemNumberData.IsItemStackable)
	{
		ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
// on click for drag and using item
FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// for weapon or item, right click for usage
		if (ItemReference->ItemType == EItemType::Weapon)
		{
			UInventoryComponent* OwningInventory = ItemReference->OwningInventory;
			OwningInventory->UseWeapon(ItemReference);
		}
		else
		{
			UInventoryComponent* OwningInventory = ItemReference->OwningInventory;
			OwningInventory->UseItem(ItemReference);
		}
		return Reply.Handled();
	}
	
	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation; // performing actions when drag and drop has been detected
	}
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
