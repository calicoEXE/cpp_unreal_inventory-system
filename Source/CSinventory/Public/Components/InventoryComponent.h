// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
	ActualAmountAdded(0),
	ItemOperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	// actual amount of item added to the inventory
	UPROPERTY(BlueprintReadOnly, Category = "Item Adding Results")
	int32 ActualAmountAdded;
	// enum after end of item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Adding Results")
	EItemAddResult ItemOperationResult;
	// additional info passed with result
	UPROPERTY(BlueprintReadOnly, Category = "Item Adding Results")
	FText ResultMessage;

	// giving indicators for all item adding results
	static FItemAddResult AddedNone(const FText & ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0 ;
		AddedNoneResult.ItemOperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}
	static FItemAddResult AddedPartial(const int PartialAmountAdded, const FText & ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded ;
		AddedPartialResult.ItemOperationResult = EItemAddResult::IAR_PartialItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}
	static FItemAddResult AddedAll(const int32 FullAmountAdded, const FText & SuccessMessage)
	{
		FItemAddResult AddedFullResult;
		AddedFullResult.ActualAmountAdded = FullAmountAdded ;
		AddedFullResult.ItemOperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedFullResult.ResultMessage = SuccessMessage;
		return AddedFullResult;
	}
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CSINVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES ===============================================================
	
	FOnInventoryUpdated OnInventoryUpdated;

	// FUNCTIONS ============================================================================

	UInventoryComponent();

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* ItemInput);

	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemInInventory) const;
	UFUNCTION(Category = "Inventory")
	UItemBase* FindItemByID(UItemBase* ItemInInventory) const; // check against ID from ItemBase
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemInInventory) const; 

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove); // drop all from inv
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemInInventory, int32 DesiredAmountToRemove); // drop some items from inv
	UFUNCTION(Category = "Inventory")
	void SplittingStack(UItemBase* ItemInInventory, const int32 AmountToSplit);

	// item/weapon usage
	void UseItem(UItemBase* ItemUsed);
	virtual void UseWeapon(UItemBase* ItemUsed);

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetStackCapacity() const { return InventoryWeightCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetInventorySlotsCapacity() const { return InventorySlotsCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };


	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetInventorySlotsCapacity(const int32 NewInventorySlotsCapacity) { InventorySlotsCapacity = NewInventorySlotsCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetInventoryWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; }; // used for player upgrades
	
protected:
	// PROPERTIES & VARIABLES ===============================================================

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	
	// FUNCTIONS ============================================================================
	
	virtual void BeginPlay() override;

	// invSys check to see if player is able to add item into inventory
	FItemAddResult HandleNonStackableItems(UItemBase* ItemInInventory);
	int32 HandleStackableItems(UItemBase* ItemInInventory, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UItemBase* ItemInInventory, int32 RequestedAddAmount);
	int32 CalculateAmountForFullStack(UItemBase* ExistingStackableItem, int32 InitialRequestedAddAmount); // calculate between the amount player wants to add and existing item's amount in inv against its full stack capacity

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);
		
};
