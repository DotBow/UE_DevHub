// Copyright (C) Dreamer's Tail

#pragma once

#include "Engine/DataAsset.h"
#include "Engine/EngineBaseTypes.h"
#include "Input/CommonUIInputSettings.h"
#include "EasyInputBindings.generated.h"


USTRUCT(NotBlueprintable, NotBlueprintType)
struct FEasyInputBinding
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FString FunctionName;

public:
	FEasyInputBinding(
		const FString& InFunction =
			FString("Function not provided"))
		: FunctionName(InFunction) {}

	FName GetFunctionName() const
	{
		return *FunctionName;
	}

#if WITH_EDITOR
	void SetFunctionName(
		const FString& InName)
	{
		FunctionName = InName;
	}

	static FName GetFunctionMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputBinding, FunctionName);
	}
#endif
};


USTRUCT(NotBlueprintable, NotBlueprintType)
struct FEasyInputActionKey
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FKey Key;

	UPROPERTY(EditAnywhere)
	bool bShift;

	UPROPERTY(EditAnywhere)
	bool bCtrl;

	UPROPERTY(EditAnywhere)
	bool bAlt;

public:
	FEasyInputActionKey(
		const FKey& InKey = {},
		const bool bInShift = false,
		const bool bInCtrl = false,
		const bool bInAlt = false)
		: Key(InKey)
		, bShift(bInShift)
		, bCtrl(bInCtrl)
		, bAlt(bInAlt) {}

	FKey GetKey() const
	{
		return Key;
	}

	bool GetShift() const
	{
		return bShift;
	}

	bool GetCtrl() const
	{
		return bCtrl;
	}

	bool GetAlt() const
	{
		return bAlt;
	}

#if WITH_EDITOR
	static FName GetKeyMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputActionKey, Key);
	}

	static FName GetShiftMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputActionKey, bShift);
	}

	static FName GetCtrlMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputActionKey, bCtrl);
	}

	static FName GetAltMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputActionKey, bAlt);
	}
#endif
};


USTRUCT(NotBlueprintable, NotBlueprintType)
struct FEasyInputActionBinding : public FEasyInputBinding
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<FEasyInputActionKey> Keys;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EInputEvent> InputEvent;

public:
	FEasyInputActionBinding(
		const FString& InFunction =
			FString("Function not provided"),
		const TArray<FEasyInputActionKey>& InKeys =
			{ FEasyInputActionKey() },
		const TEnumAsByte<EInputEvent> InInputEvent = IE_Pressed)
		: Super(InFunction)
		, Keys(InKeys)
		, InputEvent(InInputEvent) {}

	EInputEvent GetInputEvent() const
	{
		return InputEvent;
	}

	TArray<FEasyInputActionKey> GetKeys() const
	{
		return Keys;
	}

	TArray<FEasyInputActionKey>& GetKeysRef()
	{
		return Keys;
	}

#if WITH_EDITOR
	static FName GetKeysMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputActionBinding, Keys);
	}

	static FName GetInputEventMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputActionBinding, InputEvent);
	}
#endif
};


#if WITH_EDITOR
UENUM()
enum class EEasyInputBindingType : uint8
{
	Action = 1 << 0,
	Axis   = 2 << 0,
	UI     = 3 << 0,
};
#endif


USTRUCT(NotBlueprintable, NotBlueprintType)
struct FEasyInputAxisBinding : public FEasyInputBinding
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<FKey> Keys;

	UPROPERTY(EditAnywhere)
	float Scale;

public:
	FEasyInputAxisBinding(
		const FString& InFunction =
			FString("Function not provided"),
		const TArray<FKey>& InKeys = { FKey() },
		const float InScale = 1.f)
		: Super(InFunction)
		, Keys(InKeys)
		, Scale(InScale) {}

	float GetScale() const
	{
		return Scale;
	}

	TArray<FKey> GetKeys() const
	{
		return Keys;
	}

	TArray<FKey>& GetKeysRef()
	{
		return Keys;
	}

#if WITH_EDITOR
	static FName GetKeysMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputAxisBinding, Keys);
	}

	static FName GetScaleMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			FEasyInputAxisBinding, Scale);
	}
#endif
};


UCLASS(NotBlueprintable, NotBlueprintType)
class EASYINPUT_API UEasyInputBindings : public UDataAsset
{
	GENERATED_BODY()

public:
	UEasyInputBindings();

private:
#if WITH_EDITORONLY_DATA
	EEasyInputBindingType ActiveBindingType = EEasyInputBindingType::Action;
	FSimpleMulticastDelegate OnCategoryChanged;

	UPROPERTY(EditAnywhere, meta=(DisallowCreateNew,
		AllowedClasses="/Script/Engine.Actor"))
	TSoftClassPtr<AActor> FunctionsSource;
#endif

	UPROPERTY(EditAnywhere, meta=(NoElementDuplicate, EditFixedOrder))
	TArray<FEasyInputActionBinding> ActionBindings;

	UPROPERTY(EditAnywhere, meta=(NoElementDuplicate, EditFixedOrder))
	TArray<FEasyInputAxisBinding> AxisBindings;

	UPROPERTY(EditAnywhere, Transient, meta=(NoElementDuplicate, EditFixedOrder))
	TArray<FUIInputAction> UIActionBindings;

public:
#if WITH_EDITOR
	EEasyInputBindingType GetActiveBindingType() const
	{
		return ActiveBindingType;
	}

	void SetActiveBindingType(
		const EEasyInputBindingType InActiveBindingType)
	{
		ActiveBindingType = InActiveBindingType;
	}

	FSimpleMulticastDelegate& GetOnCategoryChanged()
	{
		return OnCategoryChanged;
	}

	TSoftClassPtr<AActor> GetFunctionsSource() const
	{
		return FunctionsSource;
	};
#endif

	const TArray<FEasyInputActionBinding>& GetActionBindings() const
	{
		return ActionBindings;
	}

	const TArray<FEasyInputAxisBinding>& GetAxisBindings() const
	{
		return AxisBindings;
	}

	static const TArray<FUIInputAction>& GetUIActionBindings();

#if WITH_EDITOR
	static FName GetFunctionSourceMember()
	{
		return GET_MEMBER_NAME_CHECKED(
			UEasyInputBindings, FunctionsSource);
	}

	void LoadUIActionBindings();
	void SaveUIInputSettings();

	static FName GetBindingsMember(
		const EEasyInputBindingType Type)
	{
		if (Type == EEasyInputBindingType::Action)
			return GET_MEMBER_NAME_CHECKED(UEasyInputBindings, ActionBindings);
		if (Type == EEasyInputBindingType::Axis)
			return GET_MEMBER_NAME_CHECKED(UEasyInputBindings, AxisBindings);
		if (Type == EEasyInputBindingType::UI)
			return GET_MEMBER_NAME_CHECKED(UEasyInputBindings, UIActionBindings);

		return NAME_None;
	}

	void AddBinding(
		const EEasyInputBindingType Type)
	{
		Modify();

		if (Type == EEasyInputBindingType::Action)
			ActionBindings.Add(FEasyInputActionBinding());
		else if (Type == EEasyInputBindingType::Axis)
			AxisBindings.Add(FEasyInputAxisBinding());
		else if (Type == EEasyInputBindingType::UI)
			UIActionBindings.Add(FUIInputAction());
	}

	void DeleteBinding(
		const int32 BindingIdx,
		const EEasyInputBindingType Type)
	{
		Modify();

		if (Type == EEasyInputBindingType::Action)
			ActionBindings.RemoveAt(BindingIdx);
		else if (Type == EEasyInputBindingType::Axis)
			AxisBindings.RemoveAt(BindingIdx);
		else if (Type == EEasyInputBindingType::UI)
			UIActionBindings.RemoveAt(BindingIdx);
	}

	void EmptyBindings(
		const EEasyInputBindingType Type)
	{
		Modify();

		if (Type == EEasyInputBindingType::Action)
			ActionBindings.Empty();
		else if (Type == EEasyInputBindingType::Axis)
			AxisBindings.Empty();
		else if (Type == EEasyInputBindingType::UI)
			UIActionBindings.Empty();
	}

	void AddBindingKey(
		const int32 BindingKey,
		const EEasyInputBindingType Type)
	{
		Modify();

		if (Type == EEasyInputBindingType::Action)
			ActionBindings[BindingKey].GetKeysRef().Add(FEasyInputActionKey());
		else if (Type == EEasyInputBindingType::Axis)
			AxisBindings[BindingKey].GetKeysRef().Add(FKey());
		else if (Type == EEasyInputBindingType::UI)
			UIActionBindings[BindingKey].KeyMappings.Add(FUIActionKeyMapping());
	}

	void DeleteBindingKey(
		const int32 BindingKey,
		const int32 KeyIdx,
		const EEasyInputBindingType Type)
	{
		if (CanDeleteBindingKey(BindingKey, Type))
		{
			Modify();

			if (Type == EEasyInputBindingType::Action)
				ActionBindings[BindingKey].GetKeysRef().RemoveAt(KeyIdx);
			else if (Type == EEasyInputBindingType::Axis)
				AxisBindings[BindingKey].GetKeysRef().RemoveAt(KeyIdx);
			else if (Type == EEasyInputBindingType::UI)
				UIActionBindings[BindingKey].KeyMappings.RemoveAt(KeyIdx);
		}
	}

	bool CanDeleteBindingKey(
		const int32 BindingKey,
		const EEasyInputBindingType Type) const
	{
		if (Type == EEasyInputBindingType::Action)
			return ActionBindings[BindingKey].GetKeys().Num() > 1;
		if (Type == EEasyInputBindingType::Axis)
			return AxisBindings[BindingKey].GetKeys().Num() > 1;
		if (Type == EEasyInputBindingType::UI)
			return UIActionBindings[BindingKey].KeyMappings.Num() > 1;

		return false;
	}

	void SetBindingFunction(
		const int32 BindingKey,
		const EEasyInputBindingType Type,
		const FString& InFunction)
	{
		Modify();

		if (Type == EEasyInputBindingType::Action)
			ActionBindings[BindingKey].SetFunctionName(InFunction);
		if (Type == EEasyInputBindingType::Axis)
			AxisBindings[BindingKey].SetFunctionName(InFunction);
	}

	TArray<FString> GetSourceFunctions(
		const EEasyInputBindingType Type) const;
#endif
};
