// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/InvHUD.h"
#include "UserInterface/MainInvMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"

AInvHUD::AInvHUD()
{
}

void AInvHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainInvMenuClass)
	{
		MainInvMenuWidget = CreateWidget<UMainInvMenu>(GetWorld(), MainInvMenuClass);
		MainInvMenuWidget->AddToViewport(5);
		MainInvMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void AInvHUD::DisplayMenu()
{
	if (MainInvMenuWidget)
	{
		bIsMenuInvVisible = true;
		MainInvMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void AInvHUD::HideMenu()
{
	if (MainInvMenuWidget)
	{
		bIsMenuInvVisible = false;
		MainInvMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInvHUD::ToggleMenu()
{
	if(bIsMenuInvVisible)
	{
		UE_LOG(LogTemp, Log, TEXT("Menu Hidden."));
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Menu Visible."));
		DisplayMenu();
		
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
	
}

void AInvHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInvHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInvHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}
