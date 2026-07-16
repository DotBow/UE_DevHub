// Copyright (C) Dreamer's Tail

#include "EasyInputComponent.h"

#include "CommonUserWidget.h"
#include "EasyInput.h"
#include "EasyInputBindings.h"
#include "Components/InputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerInput.h"
#include "Input/CommonUIInputTypes.h"


UEasyInputComponent::UEasyInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEasyInputComponent::RegisterWidgetInputs(
	UCommonUserWidget* Widget)
{
	if (const APlayerController* PlayerController = Widget->GetOwningPlayer())
	{
		if (const UEasyInputComponent* EasyInputComponent =
			PlayerController->FindComponentByClass<UEasyInputComponent>())
		{
			EasyInputComponent->RegisterWidgetInputs_Imp(Widget);
			return;
		}
	}

	UE_LOG(LogEasyInput, Error, TEXT("Failed to register widget inputs!"))
}

TObjectPtr<UEasyInputBindings> UEasyInputComponent::GetActiveInputBindings() const
{
	if (ActiveInputIndex < InputBindings.Num())
		return InputBindings[ActiveInputIndex];

	return nullptr;
}

void UEasyInputComponent::SetActiveInputBindingIndex(
	const int32 InActiveInputIndex,
	UObject* InObject,
	const bool bSetInputBindings)
{
	if (InActiveInputIndex < InputBindings.Num())
	{
		ActiveInputIndex = InActiveInputIndex;

		if (bSetInputBindings && InObject)
			SetInputBindings(InObject);
	}
}

void UEasyInputComponent::RegisterWidgetInputs_Imp(
	UCommonUserWidget* Widget) const
{
	if (const TObjectPtr<UEasyInputBindings> ActiveInputBindings = GetActiveInputBindings())
	{
		for (const FUIInputAction& InputAction : ActiveInputBindings->GetUIActionBindings())
		{
			const FString FunctionName =
				InputAction.ActionTag.GetTagName().ToString().Replace(
					TEXT("."), TEXT("_"));
			FSimpleDelegate Delegate;
			Delegate.BindUFunction(Widget, *FunctionName);
			const FUIActionBindingHandle Handle =
				Widget->RegisterUIActionBinding(FBindUIActionArgs(
				InputAction.ActionTag, false,
				Delegate));
		}
	}
}

void UEasyInputComponent::BeginPlay()
{
	Super::BeginPlay();
	SetInputBindings();
}

void UEasyInputComponent::SetInputBindings(
	UObject* InObject) const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (!ensureMsgf(PlayerController,
		TEXT("UEasyInputComponent requires Player Controller outer!")))
		return;

	if (!ensureMsgf(GetActiveInputBindings(),
		TEXT("Input Bindings are invalid!")))
		return;

	if (InObject == nullptr)
	{
		InObject = PlayerController->GetPawn();

		if (!ensureMsgf(InObject,
			TEXT("Pawn is invalid")))
			return;
	}

	SetInputBindings(
		PlayerController, InObject);
}

void UEasyInputComponent::SetInputBindings(
	const TObjectPtr<APlayerController>& PlayerController,
	UObject* InObject) const
{
	if (const TObjectPtr<UEasyInputBindings> ActiveInputBindings = GetActiveInputBindings())
	{
		PlayerController->InputComponent->ClearActionBindings();
		PlayerController->PlayerInput->ActionMappings.Empty();

		for (const FEasyInputActionBinding& ActionBinding :
			ActiveInputBindings->GetActionBindings())
		{
			for (const FEasyInputActionKey& Key :
				ActionBinding.GetKeys())
			{
				FInputKeyBinding KB(FInputChord(
					Key.GetKey(), Key.GetShift(),
					Key.GetCtrl(), Key.GetAlt(), false),
					ActionBinding.GetInputEvent());
				KB.KeyDelegate.BindDelegate(
					InObject, ActionBinding.GetFunctionName());
				PlayerController->InputComponent->KeyBindings.Emplace(
					MoveTemp(KB));
			}
		}

		PlayerController->InputComponent->ClearAxisBindings();
		PlayerController->PlayerInput->AxisMappings.Empty();

		for (const FEasyInputAxisBinding& AxisBinding :
			ActiveInputBindings->GetAxisBindings())
		{
			for (const FKey& Key : AxisBinding.GetKeys())
			{
				FInputAxisKeyMapping Mapping(
					AxisBinding.GetFunctionName(),
					Key,
					AxisBinding.GetScale());
				PlayerController->PlayerInput->AxisMappings.Emplace(
					MoveTemp(Mapping));

				FInputAxisBinding AB(AxisBinding.GetFunctionName());
				AB.AxisDelegate.BindDelegate(
					InObject, AxisBinding.GetFunctionName());
				PlayerController->InputComponent->AxisBindings.Emplace(
					MoveTemp(AB));
			}
		}

		PlayerController->PlayerInput->ForceRebuildingKeyMaps(false);
	}
}
