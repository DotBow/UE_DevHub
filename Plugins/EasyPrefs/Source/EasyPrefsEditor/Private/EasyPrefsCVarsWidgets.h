// Copyright (C) Dreamer's Tail

#pragma once
#include "EasyPrefsCVars.h"


class SEasyPrefsCVars : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEasyPrefsCVars) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArg);
};


class SEasyPrefsCVar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEasyPrefsCVar) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArg, const int32 InCVarIdx);

private:
	int32 CVarIdx = -1;
};


class SEasyPrefsCVarValue : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEasyPrefsCVarValue) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArg, const int32 InCVarIdx);

private:
	int32 CVarIdx = -1;
	TArray<FName> EnumNames;

	int32 GetIntValue() const;
	void OnIntValueChanged(int32 InValue) const;

	float GetFloatValue() const;
	void OnFloatValueChanged(float InValue) const;

	FName GetInitiallySelectedEnum() const;
	TSharedPtr<class SEasyPrefsCVarEnumText> SelectedEnumText;
	TSharedRef<SWidget> OnGenerateEnumCVarRow(FName EnumName);
	void OnEnumCVarChanged(const FName InEnumName, ESelectInfo::Type SelectInfo) const;
};


class SEasyPrefsCVarEnumText : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEasyPrefsCVarEnumText) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArg, const int32 InCVarIdx);
	void Draw();

private:
	int32 CVarIdx = -1;
};