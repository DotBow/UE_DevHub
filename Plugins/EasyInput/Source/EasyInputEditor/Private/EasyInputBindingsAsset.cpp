// Copyright (C) Dreamer's Tail

#include "EasyInputBindingsAsset.h"
#include "EasyInputEditorStyle.h"
#include "ISinglePropertyView.h"
#include "EasyInputBindingsCustomization.h"
#include "Engine/DataAsset.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScrollBox.h"


void SEasyInputDetails::Construct(
	const FArguments& InArgs,
	UEasyInputBindings* InInputBindings)
{
	InputBindings = InInputBindings;

	if (InputBindings)
	{
		InputBindings->GetOnCategoryChanged().AddSP(
			this, &SEasyInputDetails::Draw);
		Draw();
	}
}

void SEasyInputDetails::Draw()
{
	FPropertyEditorModule& PropertyEditorModule =
		FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bAllowMultipleTopLevelObjects = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowObjectLabel = false;
	DetailsViewArgs.bShowScrollBar = false;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	DetailsView->RegisterInstancedCustomPropertyLayout(
		UEasyInputBindings::StaticClass(),
		FOnGetDetailCustomizationInstance::CreateStatic(
			&FEasyInputBindingsCustomization::MakeInstance));
	DetailsView->SetObject(InputBindings, true);

	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SEasyInputCategories, InputBindings)
		]
		+SVerticalBox::Slot()
		.FillHeight(1)
		[
			DetailsView.ToSharedRef()
		]
	];
}


void SEasyInputCategories::Construct(
	const FArguments& InArgs,
	UEasyInputBindings* InInputBindings)
{
	InputBindings = InInputBindings;
	Draw();
}

void SEasyInputCategories::Draw()
{
	const TSharedPtr<SHorizontalBox> HorizontalBox =
		SNew(SHorizontalBox);

	HorizontalBox->AddSlot()
	.AutoWidth()
	[
		SNew(SEasyInputBindingType, InputBindings)
		.BindingType(EEasyInputBindingType::Action)
		.Label(FText::FromString("Action Bindings"))
		.Image(FAppStyle::GetBrush("GraphEditor.KeyEvent_16x"))
	];

	HorizontalBox->AddSlot()
	.AutoWidth()
	[
		SNew(SEasyInputBindingType, InputBindings)
		.BindingType(EEasyInputBindingType::Axis)
		.Label(FText::FromString("Axis Bindings"))
		.Image(FAppStyle::GetBrush("GraphEditor.PadEvent_16x"))
	];

	HorizontalBox->AddSlot()
	.AutoWidth()
	[
		SNew(SEasyInputBindingType, InputBindings)
		.BindingType(EEasyInputBindingType::UI)
		.Label(FText::FromString("UI Action Bindings"))
		.Image(FAppStyle::GetBrush("ClassIcon.UserWidget"))
	];

	HorizontalBox->AddSlot()
	.FillWidth(1);

	ChildSlot
	.Padding(0, 2)
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("Docking.Border"))
		.BorderBackgroundColor(FSlateColor::UseForeground())
		.HAlign(HAlign_Fill)
		.Padding(4)
		[
			HorizontalBox.ToSharedRef()
		]
	];
}


void SEasyInputBindingType::Construct(
	const FArguments& InArgs,
	UEasyInputBindings* InInputBindings)
{
	BindingType = InArgs._BindingType;
	InputBindings = InInputBindings;

	const FName StyleName = InputBindings->GetActiveBindingType() ==
		BindingType ? "PrimaryButton" : "SimpleButton";

	ChildSlot
	[
		SNew(SButton)
		.ButtonStyle(&FEasyInputEditorStyle::Get(), StyleName)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.ContentPadding(FMargin(
			4, 2, 4, 2))
		.Cursor(EMouseCursor::Hand)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SNew(SImage)
				.Image(InArgs._Image)
				.DesiredSizeOverride(
					FVector2D(16, 16))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SBox)
				.WidthOverride(128)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Font(FCoreStyle::GetDefaultFontStyle(
						"Regular", 10))
					.Text(InArgs._Label)
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
		]
		.OnClicked(this,
			&SEasyInputBindingType::OnClicked)
	];
}

FReply SEasyInputBindingType::OnClicked() const
{
	if (InputBindings)
	{
		InputBindings->SetActiveBindingType(BindingType);
		InputBindings->GetOnCategoryChanged().Broadcast();
	}

	return FReply::Handled();
}


const FName FEasyInputBindingsToolkit::DetailsTabId =
	TEXT("InputBindingsEditorDetailsTab");


void FEasyInputBindingsToolkit::InitEditor(
	const TArray<UObject*>& InObjects)
{
	InputBindings = Cast<UEasyInputBindings>(InObjects[0]);
	InputBindings->LoadUIActionBindings();

	const TSharedRef<FTabManager::FLayout> Layout =
		FTabManager::NewLayout("InputBindingsEditorLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(1.f)
				->AddTab(DetailsTabId, ETabState::OpenedTab)
			)
		)
	);

	InitAssetEditor(
		EToolkitMode::Standalone, {},
		"InputBindingsEditorLayout", Layout,
		true, true, InObjects);
}

void FEasyInputBindingsToolkit::OnClose()
{
	InputBindings->SaveUIInputSettings();
}

void FEasyInputBindingsToolkit::RegisterTabSpawners(
	const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory =
		InTabManager->AddLocalWorkspaceMenuCategory(GetBaseToolkitName());

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	/* Create details panel */
	Details = SNew(SEasyInputDetails, InputBindings);

	InTabManager->RegisterTabSpawner(DetailsTabId,
		FOnSpawnTab::CreateLambda([this] (const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
		[
			SNew(SScrollBox)
			.Orientation(Orient_Vertical)
			+SScrollBox::Slot()
			[
				Details.ToSharedRef()
			]
		];
	}))
	.SetDisplayName(INVTEXT("Details"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	/* Extend toolbar */
	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		ToolkitCommands,
		FToolBarExtensionDelegate::CreateRaw(this,
			&FEasyInputBindingsToolkit::AddToolbarExtension)
	);

	AddToolbarExtender(ToolbarExtender);
}

void FEasyInputBindingsToolkit::UnregisterTabSpawners(
	const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(DetailsTabId);
}

void FEasyInputBindingsToolkit::AddToolbarExtension(
	FToolBarBuilder& ToolbarBuilder) const
{
	FPropertyEditorModule& PropertyEditorModule =
		FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FSinglePropertyParams SinglePropertyParams;
	SinglePropertyParams.NamePlacement = EPropertyNamePlacement::Hidden;
	const TSharedPtr<ISinglePropertyView> FunctionSourcePropertyView =
		PropertyEditorModule.CreateSingleProperty(
			InputBindings,
			UEasyInputBindings::GetFunctionSourceMember(),
			SinglePropertyParams);

	ToolbarBuilder.AddWidget(
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(4)
		[
			SNew(SImage)
			.DesiredSizeOverride(FVector2D(16, 16))
			.Image(FAppStyle::Get().GetBrush("ClassIcon.Character"))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(256)
			[
				FunctionSourcePropertyView.ToSharedRef()
			]
		],
		NAME_None, false, HAlign_Center);
}


UEasyInputBindingsFactory::UEasyInputBindingsFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UEasyInputBindings::StaticClass();
}

bool UEasyInputBindingsFactory::ConfigureProperties()
{
	return true;
}

UObject* UEasyInputBindingsFactory::FactoryCreateNew(
	UClass* Class,
	UObject* InParent,
	FName Name,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn)
{
	return NewObject<UDataAsset>(
		InParent,
		UEasyInputBindings::StaticClass(),
		Name,
		Flags | RF_Transactional);
}

void FEasyInputBindingsActions::OpenAssetEditor(
	const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	MakeShared<FEasyInputBindingsToolkit>()->InitEditor(InObjects);
}
