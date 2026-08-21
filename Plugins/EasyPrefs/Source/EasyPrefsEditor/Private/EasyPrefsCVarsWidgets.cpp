// Copyright (C) Dreamer's Tail

#include "EasyPrefsCVarsWidgets.h"
#include "EasyPrefsSettings.h"
#include "Widgets/Input/SSpinBox.h"


void SEasyPrefsCVars::Construct(const FArguments& InArg)
{
	const TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox);

	if (const UEasyPrefsSettings* EasyPrefsSettings = UEasyPrefsSettings::Get())
	{
		for (int32 CVarIdx = 0; CVarIdx < EasyPrefsSettings->GetNumCVars(); ++CVarIdx)
		{
			VerticalBox->AddSlot()
			.AutoHeight()
			[
				SNew(SEasyPrefsCVar, CVarIdx)
			];
		}
	}

	ChildSlot
	[
		SNew(SBorder)
		.Padding(4)
		[
			VerticalBox
		]
	];
}

void SEasyPrefsCVar::Construct(const FArguments& InArg, const int32 InCVarIdx)
{
	CVarIdx = InCVarIdx;

	const FEasyPrefsCVar* CVarPtr =
		UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVar>(CVarIdx);

	if (!CVarPtr)
		return;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4)
		[
			SNew(SBox)
			.WidthOverride(128)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Left)
				.Text(FText::FromString(CVarPtr->GetDisplayName()))
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4)
		[
			SNew(SBox)
			.WidthOverride(128)
			[
				SNew(SEasyPrefsCVarValue, CVarIdx)
			]
		]
	];
}


void SEasyPrefsCVarValue::Construct(const FArguments& InArg, const int32 InCVarIdx)
{
	CVarIdx = InCVarIdx;

	if (UEasyPrefsSettings::Get()->IsCVarChildOf(CVarIdx, FEasyPrefsCVarInt::StaticStruct()))
	{
		const FEasyPrefsCVarInt* CVarInt =
			UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarInt>(CVarIdx);

		ChildSlot
		[
			SNew(SBox)
			.WidthOverride(64)
			[
				SNew(SSpinBox<int32>)
				.MinValue(CVarInt->MinValue)
				.MaxValue(CVarInt->MaxValue)
				.Value_Raw(this, &SEasyPrefsCVarValue::GetIntValue)
				.OnValueChanged_Raw(this, &SEasyPrefsCVarValue::OnIntValueChanged)
			]
		];
	}
	else
	{
		if (UEasyPrefsSettings::Get()->IsCVarChildOf(CVarIdx, FEasyPrefsCVarFloat::StaticStruct()))
		{
			const FEasyPrefsCVarFloat* CVarFloat =
				UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarFloat>(CVarIdx);

			ChildSlot
			[
				SNew(SBox)
				.WidthOverride(64)
				[
					SNew(SSpinBox<float>)
					.MinValue(CVarFloat->MinValue)
					.MaxValue(CVarFloat->MaxValue)
					.Value_Raw(this, &SEasyPrefsCVarValue::GetFloatValue)
					.OnValueChanged_Raw(this, &SEasyPrefsCVarValue::OnFloatValueChanged)
				]
			];
		}
	}
}

int32 SEasyPrefsCVarValue::GetIntValue() const
{
	if (const FEasyPrefsCVarInt* CVarInt =
		UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarInt>(CVarIdx))
		return CVarInt->Value;

	return -1;
}

void SEasyPrefsCVarValue::OnIntValueChanged(int32 InValue) const
{
	if (FEasyPrefsCVarInt* CVarInt =
		UEasyPrefsSettings::GetMutable()->GetCVarMutablePtr<FEasyPrefsCVarInt>(CVarIdx))
	{
		CVarInt->Value = InValue;
		CVarInt->Apply();
	}
}

float SEasyPrefsCVarValue::GetFloatValue() const
{
	if (const FEasyPrefsCVarInt* CVarInt =
		UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarInt>(CVarIdx))
		return CVarInt->Value;

	return -1;
}

void SEasyPrefsCVarValue::OnFloatValueChanged(float InValue) const
{
	if (FEasyPrefsCVarFloat* CVarFloat =
		UEasyPrefsSettings::GetMutable()->GetCVarMutablePtr<FEasyPrefsCVarFloat>(CVarIdx))
	{
		UEasyPrefsSettings::GetMutable()->Modify();
		CVarFloat->Value = InValue;
		CVarFloat->Apply();
	}
}
