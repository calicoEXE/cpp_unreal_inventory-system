// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InvHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainInvMenu;

UCLASS()
class CSINVENTORY_API AInvHUD : public AHUD
{
	GENERATED_BODY()

public:

	// PROPERTIES & VARIABLES ==================================================

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainInvMenu> MainInvMenuClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuInvVisible;
	
	// FUNCTIONS ===============================================================

	AInvHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:

	// PROPERTIES & VARIABLES ==================================================
	UPROPERTY()
	UMainInvMenu* MainInvMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	
	// FUNCTIONS ===============================================================

	virtual void BeginPlay() override;

	
};
