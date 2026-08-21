// Copyright (C) Dreamer's Tail

#pragma once
#include "StructUtils/InstancedStruct.h"
#include "EasyPrefsCVars.generated.h"


USTRUCT()
struct FEasyPrefsCVar
{
	GENERATED_BODY()

protected:
	UPROPERTY(Config, EditAnywhere)
	FString DisplayName;

	UPROPERTY(Config, EditAnywhere)
	FString Name;

public:
	FEasyPrefsCVar(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""))
		: DisplayName(InDisplayName)
		, Name(InName) {}

	virtual ~FEasyPrefsCVar() = default;

	FString GetDisplayName() const
	{
		return DisplayName;
	}

	FString GetName() const
	{
		return Name;
	}

	virtual FString GetValue() const
	{
		return FString(TEXT(""));
	}

	virtual void Apply() const;
};


USTRUCT()
struct FEasyPrefsCVarInt : public FEasyPrefsCVar
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere)
	int32 Value;

	UPROPERTY(Config, EditAnywhere)
	int32 MinValue;

	UPROPERTY(Config, EditAnywhere)
	int32 MaxValue;

	FEasyPrefsCVarInt(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""),
		const int32 InValue = 0,
		const int32 InMinValue = 0,
		const int32 InMaxValue = 1)
	{
		DisplayName = InDisplayName;
		Name = InName;
		Value = InValue;
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}

	virtual FString GetValue() const override
	{
		return FString::FromInt(Value);
	}
};


USTRUCT()
struct FEasyPrefsCVarFloat : public FEasyPrefsCVar
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere)
	float Value;

	UPROPERTY(Config, EditAnywhere)
	float MinValue;

	UPROPERTY(Config, EditAnywhere)
	float MaxValue;

	FEasyPrefsCVarFloat(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""),
		const float InValue = 0.f,
		const float InMinValue = 0.f,
		const float InMaxValue = 1.f)
	{
		DisplayName = InDisplayName;
		Name = InName;
		Value = InValue;
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}

	virtual FString GetValue() const override
	{
		return FString::SanitizeFloat(Value);
	}
};


USTRUCT()
struct FEasyPrefsCVarEnum : public FEasyPrefsCVar
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere)
	int32 Value;

	FEasyPrefsCVarEnum(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""),
		const int32 InValue = 0)
	{
		DisplayName = InDisplayName;
		Name = InName;
		Value = InValue;
	}

	virtual FString GetValue() const override
	{
		return FString::FromInt(Value);
	}
};


USTRUCT()
struct FEasyPrefsCVars
{
	GENERATED_BODY()


};
