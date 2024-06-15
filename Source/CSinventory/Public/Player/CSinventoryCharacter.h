// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CSInventory/Public/Interfaces/InteractionInterface.h"
#include "CSinventoryCharacter.generated.h"

class UItemBase;
class UInventoryComponent;
class AInvHUD;

USTRUCT()
struct FInteractionData // info about interaction
{
	GENERATED_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};
	
	UPROPERTY()
	AActor* CurrentInteractable; // currently picked up, cross checks with targetInteractable

	UPROPERTY()
	float LastInteractionCheckTime;
	
};

UCLASS(config=Game)
class ACSinventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES ===============================================================
	
	// FUNCTIONS ============================================================================

	ACSinventoryCharacter(); 
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;
	
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

	FInteractionData InteractionData;

protected:
	// PROPERTIES & VARIABLES ===============================================================
	UPROPERTY()
	AInvHUD* HUD;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable; //  data of item needed, cross checks constantly with currentInteractable

	UPROPERTY(VisibleAnywhere, Category = " Character | Inventory")
	UInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;
	
	// FUNCTIONS ============================================================================

	void ToggleMenu();

	// updated every tick to check for interaction and act accordingly
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

