#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

UENUM()
enum class EItemType : uint8
{
	Consumable UMETA(DisplayName = "Consumable"),
	Weapon UMETA(DisplayName = "Weapon"),
	Resource UMETA(DisplayName = "Resource")
};

USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	FText Type;
	UPROPERTY(EditAnywhere)
	FText InteractionText;
};

USTRUCT()
struct FItemNumData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Weight;
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;
	UPROPERTY(EditAnywhere)
	bool IsItemStackable;
};

USTRUCT()
struct FItemUsabilityCheck
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool ItemUsable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumData ItemNumberData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemUsabilityCheck ItemUsabilityCheck;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData ItemAssetData;
};
