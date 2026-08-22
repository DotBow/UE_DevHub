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
				.MinValue(CVarInt->GetMinValue())
				.MaxValue(CVarInt->GetMaxValue())
				.Value_Raw(this, &SEasyPrefsCVarValue::GetIntValue)
				.OnValueChanged_Raw(this, &SEasyPrefsCVarValue::OnIntValueChanged)
			]
		];

		return;
	}

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
				.MinValue(CVarFloat->GetMinValue())
				.MaxValue(CVarFloat->GetMaxValue())
				.Value_Raw(this, &SEasyPrefsCVarValue::GetFloatValue)
				.OnValueChanged_Raw(this, &SEasyPrefsCVarValue::OnFloatValueChanged)
			]
		];

		return;
	}

	if (UEasyPrefsSettings::Get()->IsCVarChildOf(CVarIdx, FEasyPrefsCVarEnum::StaticStruct()))
	{
		const FEasyPrefsCVarEnum* CVarEnum =
			UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarEnum>(CVarIdx);

		CVarEnum->GetEnumNames(EnumNames);
		SelectedEnumText = SNew(SEasyPrefsCVarEnumText, CVarIdx);

		ChildSlot
		[
			SNew(SBox)
			.WidthOverride(64)
			[
				SNew(SComboBox<FName>)
				.OptionsSource(&EnumNames)
				.InitiallySelectedItem(GetInitiallySelectedEnum())
				.OnGenerateWidget(this, &SEasyPrefsCVarValue::OnGenerateEnumCVarRow)
				.OnSelectionChanged(this, &SEasyPrefsCVarValue::OnEnumCVarChanged)
				.Content()
				[
					SelectedEnumText.ToSharedRef()
				]
			]
		];

		return;
	}
}

int32 SEasyPrefsCVarValue::GetIntValue() const
{
	if (const FEasyPrefsCVarInt* CVarInt =
		UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarInt>(CVarIdx))
		return CVarInt->GetIntValue();

	return -1;
}

void SEasyPrefsCVarValue::OnIntValueChanged(int32 InValue) const
{
	if (const FEasyPrefsCVarInt* CVarInt =
		UEasyPrefsSettings::GetMutable()->GetCVarMutablePtr<FEasyPrefsCVarInt>(CVarIdx))
	{
		CVarInt->SetIntValue(InValue);
		UEasyPrefsSettings::GetMutable()->SaveCVarConfig(CVarIdx);
		CVarInt->Apply();
		GEditor->RedrawAllViewports(/*bInvalidateHitProxies =*/false);
	}
}

float SEasyPrefsCVarValue::GetFloatValue() const
{
	if (const FEasyPrefsCVarFloat* CVarFloat =
		UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarFloat>(CVarIdx))
		return CVarFloat->GetFloatValue();

	return -1;
}

void SEasyPrefsCVarValue::OnFloatValueChanged(float InValue) const
{
	if (const FEasyPrefsCVarFloat* CVarFloat =
		UEasyPrefsSettings::GetMutable()->GetCVarMutablePtr<FEasyPrefsCVarFloat>(CVarIdx))
	{
		CVarFloat->SetFloatValue(InValue);
		UEasyPrefsSettings::GetMutable()->SaveCVarConfig(CVarIdx);
		CVarFloat->Apply();
		GEditor->RedrawAllViewports(/*bInvalidateHitProxies =*/false);
	}
}

FName SEasyPrefsCVarValue::GetInitiallySelectedEnum() const
{
	if (const FEasyPrefsCVarEnum* CVarEnum =
		UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarEnum>(CVarIdx))
		return CVarEnum->GetEnumName();

	return TEXT("VALUE NOT FOUND");
}

TSharedRef<SWidget> SEasyPrefsCVarValue::OnGenerateEnumCVarRow(FName EnumName)
{
	TSharedRef<STextBlock> NewSlateObject = SNew(STextBlock).Text(FText::FromName(EnumName));

	return NewSlateObject;
}

void SEasyPrefsCVarValue::OnEnumCVarChanged(const FName InEnumName, ESelectInfo::Type SelectInfo) const
{
	if (const FEasyPrefsCVarEnum* CVarEnum =
		UEasyPrefsSettings::GetMutable()->GetCVarMutablePtr<FEasyPrefsCVarEnum>(CVarIdx))
	{
		CVarEnum->SetEnumValue(InEnumName);
		UEasyPrefsSettings::GetMutable()->SaveCVarConfig(CVarIdx);
		CVarEnum->Apply();
		GEditor->RedrawAllViewports(/*bInvalidateHitProxies =*/false);

		if (SelectedEnumText.IsValid())
			SelectedEnumText->Draw();
	}
}


void SEasyPrefsCVarEnumText::Construct(const FArguments& InArg, const int32 InCVarIdx)
{
	CVarIdx = InCVarIdx;

	Draw();
}

void SEasyPrefsCVarEnumText::Draw()
{
	FName EnumName = NAME_None;

	if (const FEasyPrefsCVarEnum* CVarEnum =
		UEasyPrefsSettings::Get()->GetCVarPtr<FEasyPrefsCVarEnum>(CVarIdx))
		EnumName = CVarEnum->GetEnumName();

	ChildSlot
	[
		SNew(STextBlock).Text(FText::FromName(EnumName))
	];
}
