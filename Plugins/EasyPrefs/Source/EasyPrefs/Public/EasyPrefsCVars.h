// Copyright (C) Dreamer's Tail

#pragma once
#include "EasyPrefsCVars.generated.h"


USTRUCT()
struct EASYPREFS_API FEasyPrefsCVar
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FString DisplayName;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FName PropertyName;

public:
	FEasyPrefsCVar(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""),
		const FName& InPropertyName = TEXT(""))
		: DisplayName(InDisplayName)
		, Name(InName)
		, PropertyName(InPropertyName) {}

	virtual ~FEasyPrefsCVar() = default;

	FString GetDisplayName() const
	{
		return DisplayName;
	}

	FString GetName() const
	{
		return Name;
	}

	FName GetPropertyName() const
	{
		return PropertyName;
	}

	virtual FString GetValue() const;

	virtual void Apply() const;
};


USTRUCT()
struct EASYPREFS_API FEasyPrefsCVarInt : public FEasyPrefsCVar
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	int32 MinValue;

	UPROPERTY(EditAnywhere)
	int32 MaxValue;

public:
	FEasyPrefsCVarInt(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""),
		const FName& InPropertyName = TEXT(""),
		const int32 InMinValue = 0,
		const int32 InMaxValue = 1)
	{
		DisplayName = InDisplayName;
		Name = InName;
		PropertyName = InPropertyName;
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}

	virtual FString GetValue() const override;
	int32 GetIntValue() const;
	void SetIntValue(const int32 InValue) const;

	int32 GetMinValue() const
	{
		return MinValue;
	}

	int32 GetMaxValue() const
	{
		return MaxValue;
	}
};


USTRUCT()
struct EASYPREFS_API FEasyPrefsCVarFloat : public FEasyPrefsCVar
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float MinValue;

	UPROPERTY(EditAnywhere)
	float MaxValue;

public:
	FEasyPrefsCVarFloat(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""),
		const FName& InPropertyName = TEXT(""),
		const float InMinValue = 0.f,
		const float InMaxValue = 1.f)
	{
		DisplayName = InDisplayName;
		Name = InName;
		PropertyName = InPropertyName;
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}

	virtual FString GetValue() const override;
	int32 GetFloatValue() const;
	void SetFloatValue(const float InValue) const;

	float GetMinValue() const
	{
		return MinValue;
	}

	float GetMaxValue() const
	{
		return MaxValue;
	}
};


USTRUCT()
struct EASYPREFS_API FEasyPrefsCVarEnum : public FEasyPrefsCVarInt
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<FName, int32> EnumNames;

public:
	FEasyPrefsCVarEnum(
		const FString& InDisplayName = TEXT(""),
		const FString& InName = TEXT(""),
		const FName& InPropertyName = TEXT(""),
		const TMap<FName, int32>& InEnumNames = {})
	{
		DisplayName = InDisplayName;
		Name = InName;
		PropertyName = InPropertyName;
		EnumNames = InEnumNames;
	}

	void GetEnumNames(TArray<FName>& OutEnumNames) const;
	int32 GetEnumInt(const FName& InEnumName) const;
	FName GetEnumName() const;
	void SetEnumValue(const FName& InEnumName) const;
};
