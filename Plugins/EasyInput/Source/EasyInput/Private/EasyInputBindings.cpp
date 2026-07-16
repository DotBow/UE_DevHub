// Copyright (C) Dreamer's Tail

#include "EasyInputBindings.h"

#include "Input/CommonUIInputSettings.h"


UEasyInputBindings::UEasyInputBindings()
{
}

const TArray<FUIInputAction>& UEasyInputBindings::GetUIActionBindings()
{
	const UCommonUIInputSettings* Settings =
		GetMutableDefault<UCommonUIInputSettings>();
	return Settings->GetUIInputActions();
}

#if WITH_EDITOR
void UEasyInputBindings::LoadUIActionBindings()
{
	const UCommonUIInputSettings* Settings =
		GetMutableDefault<UCommonUIInputSettings>();
	UIActionBindings.Empty();
	UIActionBindings.Append(Settings->GetUIInputActions());
}

void UEasyInputBindings::SaveUIInputSettings()
{
	/* Get Input Actions stored in common ui settings */
	UCommonUIInputSettings* Settings = GetMutableDefault<UCommonUIInputSettings>();
	FArrayProperty* InputActionsProp = FindFProperty<FArrayProperty>(
		Settings->GetClass(), "InputActions");

	TArray<FUIInputAction>* InputActions =
		InputActionsProp->ContainerPtrToValuePtr<TArray<
			FUIInputAction>>(Settings);

	if (InputActionsProp)
	{
		auto FirePreEvent = [&](FProperty* Property)
		{
			FEditPropertyChain Event;
			Event.AddHead(Property);
			Event.AddHead(InputActionsProp);
			Event.SetActiveMemberPropertyNode(InputActionsProp);
			Event.SetActivePropertyNode(Property);
			Settings->PreEditChange(Event);
		};

		auto FirePostEvent = [&](
			FProperty* Property, const EPropertyChangeType::Type Type =
			EPropertyChangeType::ValueSet)
		{
			FPropertyChangedEvent Event(InputActionsProp, Type);
			Event.MemberProperty = InputActionsProp;
			Event.Property = Property;
			TMap<FString, int32> Map;
			Map.Add(InputActionsProp->GetName(),
				InputActions->Num() - 1);
			Event.SetArrayIndexPerObject({ Map });
			Settings->PostEditChangeProperty(Event);
		};

		/* Clear array */
		FirePreEvent(InputActionsProp);
		InputActions->Empty();
		FirePostEvent(InputActionsProp, EPropertyChangeType::ArrayClear);

		for (const FUIInputAction& UIInputAction : UIActionBindings)
		{
			FirePreEvent(InputActionsProp);
			FUIInputAction& InputAction = InputActions->AddDefaulted_GetRef();
			InputAction = UIInputAction;
			FirePostEvent(InputActionsProp, EPropertyChangeType::ArrayAdd);
		}

		/* Update ini file */
		const FString ConfigPath = FPaths::ProjectConfigDir() + TEXT("DefaultInput.ini");
		Settings->TryUpdateDefaultConfigFile(ConfigPath);
	}
}

TArray<FString> UEasyInputBindings::GetSourceFunctions(
	const EEasyInputBindingType Type) const
{
	TArray<FString> FunctionNames;

	if (!FunctionsSource.IsNull())
	{
		UClass* LoadedClass = FunctionsSource.LoadSynchronous();
		const UStruct* Struct = Cast<UStruct>(LoadedClass);

		for (TFieldIterator<UFunction> FuncIt(Struct);
			FuncIt; ++FuncIt)
		{
			if (const UFunction* Function = *FuncIt; Function &&
				Function->HasAnyFunctionFlags(FUNC_BlueprintEvent) &&
				Function->HasMetaData("EasyInputFunction"))
			{
				if ((Type == EEasyInputBindingType::Action && Function->NumParms == 0) ||
					(Type == EEasyInputBindingType::Axis && Function->NumParms > 0))
				{
					FString FunctionName = Function->GetName();
					FunctionNames.Add(FunctionName);
				}
			}
		}
	}

	return FunctionNames;
}
#endif
