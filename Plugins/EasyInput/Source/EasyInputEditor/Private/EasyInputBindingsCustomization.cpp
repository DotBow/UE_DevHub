// Copyright (C) Dreamer's Tail

#include "EasyInputBindingsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EasyInputEditorStyle.h"
#include "EasyInputBindings.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"


#define LOCTEXT_NAMESPACE "EasyInputBindingsCustomization"

TSharedRef<IPropertyTypeCustomization> FEasyInputActionKeyCustomization::MakeInstance()
{
	return MakeShareable(
		new FEasyInputActionKeyCustomization());
}

void FEasyInputActionKeyCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> StructPropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

void FEasyInputActionKeyCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle> StructPropertyHandle,
	IDetailChildrenBuilder& StructBuilder,
	IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	const int32 KeyIdx = StructPropertyHandle->GetIndexInArray();
	const TSharedPtr<IPropertyHandle> ParentHandle =
		StructPropertyHandle->GetParentHandle()->GetParentHandle();
	const int32 BindingIdx = ParentHandle->GetIndexInArray();

	TArray<UObject*> Objects;
	StructPropertyHandle->GetOuterObjects(Objects);
	InputBindings = Cast<UEasyInputBindings>(Objects[0]);

	const TSharedPtr<IPropertyHandle> KeyHandle =
		StructPropertyHandle->GetChildHandle(
			FEasyInputActionKey::GetKeyMember());
	IDetailPropertyRow& KeyPropertyRow =
		StructBuilder.AddProperty(KeyHandle.ToSharedRef());

	TSharedPtr<SWidget> OutKeyNameWidget;
	TSharedPtr<SWidget> OutKeyValueWidget;
	KeyPropertyRow.GetDefaultWidgets(
		OutKeyNameWidget, OutKeyValueWidget);
	KeyPropertyRow.Visibility(EVisibility::Collapsed);

	const TSharedPtr<IPropertyHandle> ShiftHandle =
		StructPropertyHandle->GetChildHandle(
			FEasyInputActionKey::GetShiftMember());
	const TSharedPtr<IPropertyHandle> CtrlHandle =
		StructPropertyHandle->GetChildHandle(
			FEasyInputActionKey::GetCtrlMember());
	const TSharedPtr<IPropertyHandle> AltHandle =
		StructPropertyHandle->GetChildHandle(
			FEasyInputActionKey::GetAltMember());

	FDetailWidgetRow& KeyRow = StructBuilder.AddCustomRow(
		LOCTEXT("KeyRow", "KeyRow"));

	KeyRow.WholeRowWidget
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(200)
			[
				OutKeyValueWidget.ToSharedRef()
			]
		]
		+SHorizontalBox::Slot()
		.Padding(2)
		.AutoWidth()
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SEasyInputActionKeyRow)
			.PropertyHandle(ShiftHandle)
			.Label("Shift")
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SEasyInputActionKeyRow)
			.PropertyHandle(CtrlHandle)
			.Label("Ctrl")
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SEasyInputActionKeyRow)
			.PropertyHandle(AltHandle)
			.Label("Alt")
		]
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.AutoWidth()
		[
			SNew(SBox)
			.Visibility_Lambda([this, BindingIdx] ()
			{
				return InputBindings->CanDeleteBindingKey(BindingIdx, EEasyInputBindingType::Action) ?
					EVisibility::Visible : EVisibility::Collapsed;
			})
			[
				PropertyCustomizationHelpers::MakeDeleteButton(
				FSimpleDelegate::CreateLambda(
				[this, &StructCustomizationUtils, BindingIdx, KeyIdx] ()
				{
					InputBindings->DeleteBindingKey(
						BindingIdx, KeyIdx, EEasyInputBindingType::Action);
					StructCustomizationUtils.GetPropertyUtilities()->ForceRefresh();
				}))
			]
		]
	];
}


void SEasyInputActionKeyRow::Construct(const FArguments& InArgs)
{
	PropertyHandle = InArgs._PropertyHandle;
	Label = InArgs._Label;

	Draw();
}

void SEasyInputActionKeyRow::Draw()
{
	bool bValue;
	PropertyHandle->GetValue(bValue);

	ChildSlot
	.VAlign(VAlign_Center)
	.Padding(2, 0)
	[
		SNew(SButton)
		.Text(FText::FromString(Label))
		.ButtonStyle(FAppStyle::Get(), bValue ?
			"PrimaryButton" : "Button")
		.OnClicked(this,
			&SEasyInputActionKeyRow::OnButtonClicked)
	];
}

FReply SEasyInputActionKeyRow::OnButtonClicked()
{
	bool bValue;
	PropertyHandle->GetValue(bValue);
	PropertyHandle->SetValue(!bValue);
	Draw();

	return FReply::Handled();
}


TSharedRef<IDetailCustomization> FEasyInputBindingsCustomization::MakeInstance()
{
	return MakeShareable(new FEasyInputBindingsCustomization());
}

void FEasyInputBindingsCustomization::CustomizeDetails(
	IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	InputBindings = Cast<UEasyInputBindings>(Objects[0]);

	DetailBuilder.HideCategory("EasyInputBindings");

	CustomizeBindings(DetailBuilder,
		InputBindings->GetActiveBindingType());
}

void FEasyInputBindingsCustomization::CustomizeBindings(
	IDetailLayoutBuilder& DetailBuilder,
	const EEasyInputBindingType Type) const
{
	FName CategoryName = "Unknown Category";

	if (Type == EEasyInputBindingType::Action)
		CategoryName = "Action Bindings";
	else if (Type == EEasyInputBindingType::Axis)
		CategoryName = "Axis Bindings";
	else if (Type == EEasyInputBindingType::UI)
		CategoryName = "UI Action Bindings";

	/* Create category with add and empty buttons */
	IDetailCategoryBuilder& BindingsCategory =
		DetailBuilder.EditCategory(CategoryName);
	BindingsCategory.HeaderContent(
	SNew(SHorizontalBox)
	+SHorizontalBox::Slot()
	.HAlign(HAlign_Left)
	.VAlign(VAlign_Center)
	.AutoWidth()
	[
		SNew(STextBlock)
		.Text(FText::FromName(CategoryName))
	]
	+SHorizontalBox::Slot()
	.HAlign(HAlign_Left)
	.AutoWidth()
	.Padding(4, 4, 0, 4)
	[
		PropertyCustomizationHelpers::MakeAddButton(
		FSimpleDelegate::CreateLambda(
		[this, &DetailBuilder, Type] ()
		{
			InputBindings->AddBinding(Type);
			DetailBuilder.GetPropertyUtilities().Get().ForceRefresh();
		}))
	]
	+SHorizontalBox::Slot()
	.HAlign(HAlign_Left)
	.AutoWidth()
	.Padding(0, 4, 4, 4)
	[
		PropertyCustomizationHelpers::MakeDeleteButton(
		FSimpleDelegate::CreateLambda(
		[this, &DetailBuilder, Type] ()
		{
			InputBindings->EmptyBindings(Type);
			DetailBuilder.GetPropertyUtilities().Get().ForceRefresh();
		}))
	], true);

	/* Get array of bindings */
	const TSharedRef<IPropertyHandle> BindingsHandle =
		DetailBuilder.GetProperty(UEasyInputBindings::GetBindingsMember(Type));
	const TSharedPtr<IPropertyHandleArray> BindingsArray =
		BindingsHandle->AsArray();
	uint32 NumBindings;
	BindingsArray->GetNumElements(NumBindings);

	/* If array of bindings is empty then report */
	if (NumBindings == 0)
	{
		BindingsCategory.AddCustomRow(
		FText::FromString(TEXT("BindingsEmpty")))
		.WholeRowWidget
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(32)
			[
				SNew(STextBlock)
				.Text(FText::FromString(
					CategoryName.ToString() + " is empty"))
			]
		];
	}

	/* Draw each binding in a loop */
	for (int32 BindingIdx = static_cast<int32>(NumBindings) - 1;
		BindingIdx >= 0; BindingIdx--)
	{
		const TSharedRef<IPropertyHandle> BindingHandle =
			BindingsArray->GetElement(BindingIdx);

		/*
		 * Draw binding header
		 * - functions selector/tag icon
		 * - function name/tag
		 * - delete button
		 */
		IDetailGroup& BindingGroup = BindingsCategory.AddGroup(
			FName("BindingGroup"), FText::GetEmpty());
		BindingGroup.HeaderRow()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				GetBindingHeader(BindingGroup, BindingHandle, BindingIdx, Type)
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.AutoWidth()
			.Padding(4, 4, 0, 4)
			[
				PropertyCustomizationHelpers::MakeDeleteButton(
				FSimpleDelegate::CreateLambda(
				[this, &DetailBuilder, BindingIdx, Type] ()
				{
					InputBindings->DeleteBinding(BindingIdx, Type);
					DetailBuilder.GetPropertyUtilities().Get().ForceRefresh();
				}))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
		];

		/* Draw keys array */
		IDetailGroup& KeysGroup = BindingGroup.AddGroup(
			FName("KeysGroup"), FText::GetEmpty());
		KeysGroup.HeaderRow()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(4)
			[
				SNew(SImage)
				.DesiredSizeOverride(FVector2D(16, 16))
				.Image(FEasyInputEditorStyle::Get().GetBrush(
					"EasyInputBinding.Keys"))
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Keys"))
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.AutoWidth()
			.Padding(4, 4, 0, 4)
			[
				PropertyCustomizationHelpers::MakeAddButton(
				FSimpleDelegate::CreateLambda(
				[this, &DetailBuilder, BindingIdx, Type] ()
				{
					InputBindings->AddBindingKey(BindingIdx, Type);
					DetailBuilder.GetPropertyUtilities().Get().ForceRefresh();
				}))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
		];

		/* Draw key widget for each binding type */
		if (Type == EEasyInputBindingType::Action)
		{
			const TSharedPtr<IPropertyHandle> KeysHandle =
				BindingHandle->GetChildHandle(
					FEasyInputActionBinding::GetKeysMember());
			const TSharedPtr<IPropertyHandleArray> KeysArray =
				KeysHandle->AsArray();
			uint32 NumKeys;
			KeysArray->GetNumElements(NumKeys);

			for (int32 KeyIdx = static_cast<int32>(NumKeys) - 1;
				KeyIdx >= 0; KeyIdx--)
			{
				KeysGroup.AddPropertyRow(KeysArray->GetElement(KeyIdx));
			}
		}
		else if (Type == EEasyInputBindingType::Axis)
		{
			const TSharedPtr<IPropertyHandle> KeysHandle =
				BindingHandle->GetChildHandle(
					FEasyInputAxisBinding::GetKeysMember());
			const TSharedPtr<IPropertyHandleArray> KeysArray =
				KeysHandle->AsArray();
			uint32 NumKeys;
			KeysArray->GetNumElements(NumKeys);

			for (int32 KeyIdx = static_cast<int32>(NumKeys) - 1;
				KeyIdx >= 0; KeyIdx--)
			{
				IDetailPropertyRow& KeyPropertyRow =
					KeysGroup.AddPropertyRow(KeysArray->GetElement(KeyIdx));

				TSharedPtr<SWidget> OutKeyNameWidget;
				TSharedPtr<SWidget> OutKeyValueWidget;
				KeyPropertyRow.GetDefaultWidgets(
					OutKeyNameWidget, OutKeyValueWidget);
				KeyPropertyRow.Visibility(EVisibility::Collapsed);

				KeysGroup.AddWidgetRow()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(200)
						[
							OutKeyValueWidget.ToSharedRef()
						]
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(SBox)
						.Visibility_Lambda([this, BindingIdx] ()
						{
							return InputBindings->CanDeleteBindingKey(BindingIdx, EEasyInputBindingType::Axis) ?
								EVisibility::Visible : EVisibility::Collapsed;
						})
						[
							PropertyCustomizationHelpers::MakeDeleteButton(
							FSimpleDelegate::CreateLambda(
							[this, &DetailBuilder, BindingIdx, KeyIdx] ()
							{
								InputBindings->DeleteBindingKey(
									BindingIdx, KeyIdx, EEasyInputBindingType::Axis);
								DetailBuilder.GetPropertyUtilities().Get().ForceRefresh();
							}))
						]
					]
				];
			}
		}
		else if (Type == EEasyInputBindingType::UI)
		{
			const TSharedPtr<IPropertyHandle> KeysHandle =
				BindingHandle->GetChildHandle(
					GET_MEMBER_NAME_CHECKED(FUIInputAction, KeyMappings));
			const TSharedPtr<IPropertyHandleArray> KeysArray =
				KeysHandle->AsArray();
			uint32 NumKeys;
			KeysArray->GetNumElements(NumKeys);

			for (int32 KeyIdx = static_cast<int32>(NumKeys) - 1;
				KeyIdx >= 0; KeyIdx--)
			{
				const TSharedRef<IPropertyHandle> KeyMappingHandle =
					KeysArray->GetElement(KeyIdx);
				const TSharedPtr<IPropertyHandle> KeyHandle =
					KeyMappingHandle->GetChildHandle(
						GET_MEMBER_NAME_CHECKED(FUIActionKeyMapping, Key));

				IDetailPropertyRow& KeyPropertyRow =
					KeysGroup.AddPropertyRow(KeyHandle.ToSharedRef());

				TSharedPtr<SWidget> OutKeyNameWidget;
				TSharedPtr<SWidget> OutKeyValueWidget;
				KeyPropertyRow.GetDefaultWidgets(
					OutKeyNameWidget, OutKeyValueWidget);
				KeyPropertyRow.Visibility(EVisibility::Collapsed);

				KeysGroup.AddWidgetRow()
				.WholeRowWidget
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(200)
						[
							OutKeyValueWidget.ToSharedRef()
						]
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(SBox)
						.Visibility_Lambda([this, BindingIdx] ()
						{
							return InputBindings->CanDeleteBindingKey(BindingIdx, EEasyInputBindingType::UI) ?
								EVisibility::Visible : EVisibility::Collapsed;
						})
						[
							PropertyCustomizationHelpers::MakeDeleteButton(
							FSimpleDelegate::CreateLambda(
							[this, &DetailBuilder, BindingIdx, KeyIdx] ()
							{
								InputBindings->DeleteBindingKey(
									BindingIdx, KeyIdx, EEasyInputBindingType::UI);
								DetailBuilder.GetPropertyUtilities().Get().ForceRefresh();
							}))
						]
					]
				];
			}
		}

		/* Additional event/scale property */
		if (Type != EEasyInputBindingType::UI)
		{
			BindingGroup.AddWidgetRow().WholeRowWidget
			[
				SNew(SEasyInputRow)
				.IconName("EasyInputBinding.Event")
				.PropertyHandle(BindingHandle)
				.PropertyName(Type == EEasyInputBindingType::Action ?
					FEasyInputActionBinding::GetInputEventMember() :
					FEasyInputAxisBinding::GetScaleMember())
			];
		}
	}
}

TSharedRef<SWidget> FEasyInputBindingsCustomization::GetBindingHeader(
	IDetailGroup& BindingGroup,
	const TSharedRef<IPropertyHandle>& BindingHandle,
	const int32 BindingIdx,
	const EEasyInputBindingType Type) const
{
	if (Type == EEasyInputBindingType::UI)
	{
		IDetailPropertyRow& PropertyRow =
			BindingGroup.AddPropertyRow(BindingHandle->GetChildHandle(
				GET_MEMBER_NAME_CHECKED(FUIInputAction, ActionTag)).ToSharedRef());

		TSharedPtr<SWidget> OutNameWidget;
		TSharedPtr<SWidget> OutValueWidget;
		PropertyRow.GetDefaultWidgets(
			OutNameWidget, OutValueWidget);
		PropertyRow.Visibility(EVisibility::Collapsed);

		return
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.DesiredSizeOverride(FVector2D(16, 16))
			.Image(FAppStyle::Get().GetBrush("Icons.Tag"))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(200)
			[
				OutValueWidget.ToSharedRef()
			]
		];
	}

	return
	SNew(SHorizontalBox)
	+SHorizontalBox::Slot()
	.AutoWidth()
	[
		SNew(SEasyInputFunctions)
		.InputBindings(InputBindings)
		.BindingType(Type)
		.BindingKey(BindingIdx)
	]
	+SHorizontalBox::Slot()
	.AutoWidth()
	.Padding(2)
	+SHorizontalBox::Slot()
	.AutoWidth()
	[
		SNew(SEasyInputRow)
		.PropertyHandle(BindingHandle)
		.PropertyName(FEasyInputBinding::GetFunctionMember())
	];
}


void SEasyInputRow::Construct(const FArguments& InArgs)
{
	const TSharedPtr<SHorizontalBox> Content =
		SNew(SHorizontalBox);

	if (!InArgs._IconName.IsNone())
	{
		Content->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(4)
		[
			SNew(SImage)
			.DesiredSizeOverride(FVector2D(16, 16))
			.Image(FEasyInputEditorStyle::Get().GetBrush(InArgs._IconName))
		];
	}

	if (InArgs._PropertyHandle)
	{
		if (const TSharedPtr<IPropertyHandle> ChildHandle =
			InArgs._PropertyHandle->GetChildHandle(InArgs._PropertyName))
		{
			Content->AddSlot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(200)
				[
					ChildHandle->CreatePropertyValueWidget()
				]
			];
		}
	}

	Content->AddSlot()
	.FillWidth(0.8f);

	ChildSlot
	[
		Content.ToSharedRef()
	];
}


void SEasyInputFunctions::Construct(const FArguments& InArgs)
{
	InputBindings = InArgs._InputBindings;
	BindingType = InArgs._BindingType;
	BindingKey = InArgs._BindingKey;

	for (const FString Function :
		InputBindings->GetSourceFunctions(BindingType))
	{
		Functions.Add(MakeShared<FString>(Function));
	}

	ChildSlot
	.HAlign(HAlign_Left)
	.VAlign(VAlign_Center)
	[
		SNew(SComboBox<TSharedPtr<FString>>)
		.Cursor(EMouseCursor::Hand)
		.ComboBoxStyle(FAppStyle::Get(),
			"SimpleComboBox")
		.ToolTipText(FText::FromString(
			"Select the function to use"))
		.OptionsSource(&Functions)
		.OnGenerateWidget_Static(
			&SEasyInputFunctions::OnGenerateWidget)
		.OnSelectionChanged(this,
			&SEasyInputFunctions::OnSelectionChanged)
		.HasDownArrow(false)
		.IsEnabled(this,
			&SEasyInputFunctions::IsComboEnabled)
		.ContentPadding(FMargin(0))
		[
			SNew(SImage)
			.DesiredSizeOverride(FVector2D(16, 16))
			.Image(FEasyInputEditorStyle::Get().GetBrush(
				"EasyInputBinding.Function"))
		]
	];
}

TSharedRef<SWidget> SEasyInputFunctions::OnGenerateWidget(
	TSharedPtr<FString> String)
{
	return
	SNew(STextBlock)
	.Text(FText::FromString(*String.Get()));
}

void SEasyInputFunctions::OnSelectionChanged(
	TSharedPtr<FString> String,
	ESelectInfo::Type SelectInfo) const
{
	if (!ensureMsgf(InputBindings,
		TEXT("Input Bindings is invalid!")))
		return;

	InputBindings->SetBindingFunction(
		BindingKey, BindingType, *String.Get());
}

bool SEasyInputFunctions::IsComboEnabled() const
{
	if (!ensureMsgf(InputBindings,
		TEXT("Input Bindings is invalid!")))
		return false;

	return !InputBindings->GetFunctionsSource().IsNull();
}

#undef LOCTEXT_NAMESPACE
