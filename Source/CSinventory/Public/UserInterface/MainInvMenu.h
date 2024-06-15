// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInvMenu.generated.h"

class ACSinventoryCharacter;
/**
 * 
 */
UCLASS()
class CSINVENTORY_API UMainInvMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ACSinventoryCharacter* PlayerCharacter;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	
	
protected:
};
