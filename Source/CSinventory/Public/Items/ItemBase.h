// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSInventory/Public/Data/ItemDataStructs.h"
#include "CSInventory/Public/Player/CSinventoryCharacter.h"
#include "ItemBase.generated.h"

class UInventoryComponent;

UCLASS()
class CSINVENTORY_API UItemBase : public UObject
{
public:
	// PROPERTIES & VARIABLES ===============================================================
	UPROPERTY()
	UInventoryComponent* OwningInventory;
	
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Item", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData ItemTextData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumData ItemNumberData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemUsabilityCheck ItemUsabilityCheck;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData ItemAssetData;

	UPROPERTY(VisibleAnywhere, Category = "Item Usage")
	bool bIsWeaponInUse;
	
	bool bIsItemCopy;
	bool bIsItemPickup;

	// FUNCTIONS ===============================================================

	UItemBase();

	void ResetItemFlags();

	UItemBase* CreateItemCopy(); // func that returns pointer

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetMaxItemStackWeight() const { return Quantity * ItemNumberData.Weight; };
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return ItemNumberData.Weight; };
	UFUNCTION(Category = "Item")
	FORCEINLINE bool GetMaxItemStackSize() const { return Quantity == ItemNumberData.MaxStackSize; };
	//UFUNCTION(Category = "Item")
	//FORCEINLINE bool GetItemSingleStackAmount() const { return Quantity; };

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(ACSinventoryCharacter* Character);

protected:
	bool operator=(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
	
};
