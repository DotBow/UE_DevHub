// Copyright (C) Dreamer's Tail

#include "EasyPrefsCVars.h"
#include "EasyPrefsSettings.h"


FString FEasyPrefsCVar::GetValue() const
{
	return FString(TEXT(""));
}

void FEasyPrefsCVar::Apply() const
{
	IConsoleVariable* EngineCVar =
	IConsoleManager::Get().FindConsoleVariable(*Name);

	if (EngineCVar && (EngineCVar->GetFlags() & ECVF_ReadOnly) == 0)
		EngineCVar->Set(*GetValue(), ECVF_SetByProjectSetting);
}


FString FEasyPrefsCVarInt::GetValue() const
{
	return FString::FromInt(GetIntValue());
}

int32 FEasyPrefsCVarInt::GetIntValue() const
{
	if (const FProperty* Property = FindFieldChecked<FProperty>(
		UEasyPrefsSettings::StaticClass(), PropertyName))
	{
		if (const int32* Value = Property->ContainerPtrToValuePtr<int32>(
			UEasyPrefsSettings::GetMutable()))
			return *Value;
	}

	return -1;
}

void FEasyPrefsCVarInt::SetIntValue(const int32 InValue) const
{
	if (const FProperty* Property = FindFieldChecked<FProperty>(
		UEasyPrefsSettings::StaticClass(), PropertyName))
	{
		Property->SetSingleValue_InContainer(
			UEasyPrefsSettings::GetMutable(), &InValue, 0);
	}
}


FString FEasyPrefsCVarFloat::GetValue() const
{
	return FString::SanitizeFloat(GetFloatValue());
}

int32 FEasyPrefsCVarFloat::GetFloatValue() const
{
	if (const FProperty* Property = FindFieldChecked<FProperty>(
		UEasyPrefsSettings::StaticClass(), PropertyName))
	{
		if (const float* Value = Property->ContainerPtrToValuePtr<float>(
			UEasyPrefsSettings::GetMutable()))
			return *Value;
	}

	return -1;
}

void FEasyPrefsCVarFloat::SetFloatValue(const float InValue) const
{
	if (const FProperty* Property = FindFieldChecked<FProperty>(
		UEasyPrefsSettings::StaticClass(), PropertyName))
	{
		Property->SetSingleValue_InContainer(
			UEasyPrefsSettings::GetMutable(), &InValue, 0);
	}
}

void FEasyPrefsCVarEnum::GetEnumNames(TArray<FName>& OutEnumNames) const
{
	EnumNames.GetKeys(OutEnumNames);
}

int32 FEasyPrefsCVarEnum::GetEnumInt(const FName& InEnumName) const
{
	if (EnumNames.Contains(InEnumName))
		return EnumNames[InEnumName];

	return -1;
}

FName FEasyPrefsCVarEnum::GetEnumName() const
{
	FName FoundName = NAME_None;

	if (const int32 IntValue = GetIntValue(); IntValue > -1)
	{
		for (auto It = EnumNames.CreateConstIterator(); It; ++It)
		{
			if (It.Value() == IntValue)
			{
				FoundName = It.Key();
				break;
			}
		}
	}

	return FoundName;
}

void FEasyPrefsCVarEnum::SetEnumValue(const FName& InEnumName) const
{
	if (const int32 IntValue = GetEnumInt(InEnumName); IntValue > -1)
		SetIntValue(IntValue);
}
