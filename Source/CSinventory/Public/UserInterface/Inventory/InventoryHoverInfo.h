// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "InventoryHoverInfo.generated.h"

class UTextBlock;
class UInventoryItemSlot;

UCLASS()
class CSINVENTORY_API UInventoryHoverInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	UInventoryItemSlot* HoveringOverInventorySlot;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemType;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemUsabilityCheck;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxItemStackSize; // entire maximum stack amount
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemSingleStackSizeValue; // item's current stack value IN HAND
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemSingleStackWeightValue; // item's current weight value IN HAND
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxItemStackWeight; // entire maximum stack weight
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackSizeColumn; // for stack size title
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemStackability; // check if item is stackable
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Slash;

	
protected:
	virtual void NativeConstruct() override;
};
