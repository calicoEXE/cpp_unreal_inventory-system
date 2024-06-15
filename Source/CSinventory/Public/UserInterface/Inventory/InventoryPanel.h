// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemSlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "InventoryPanel.generated.h"

class UInventoryComponent;
class UInventoryItemSlot;
class ACSinventoryCharacter;
class UWrapBox;

UCLASS()
class CSINVENTORY_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* InventoryWeightInfo;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* InventoryCapacityInfo;

	UPROPERTY()
	ACSinventoryCharacter* PlayerCharacter;
	UPROPERTY()
	UInventoryComponent* InventoryReference;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;
	
protected:

	void SetInfoText() const; // set weight & capacity info
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
