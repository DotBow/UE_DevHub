// Copyright (C) Dreamer's Tail

#include "EasyInputEditorStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Styling/StyleColors.h"


FName FEasyInputEditorStyle::StyleName("EasyInputEditorStyle");

FEasyInputEditorStyle::FEasyInputEditorStyle()
	: FSlateStyleSet(StyleName)
{
	FSlateStyleSet::SetContentRoot(
		IPluginManager::Get().FindPlugin("EasyInput")->GetBaseDir() /
		TEXT("Resources/Icons"));
	FSlateStyleSet::SetCoreContentRoot(
		FPaths::EngineContentDir() / TEXT("Editor/Slate"));

	Set("ClassThumbnail.EasyInputBindings",
		new IMAGE_BRUSH("EasyInputBindings", FVector2D(96, 96)));

	Set("EasyInputBinding.Description",
		new IMAGE_BRUSH("EasyInputBindingDescription", FVector2D(32, 32)));
	Set("EasyInputBinding.Function",
		new IMAGE_BRUSH("EasyInputBindingFunction", FVector2D(32, 32)));
	Set("EasyInputBinding.Event",
		new IMAGE_BRUSH("EasyInputBindingEvent", FVector2D(32, 32)));
	Set("EasyInputBinding.Keys",
		new IMAGE_BRUSH("EasyInputBindingKeys", FVector2D(32, 32)));

	const FButtonStyle PrimaryButton = FButtonStyle()
		.SetNormal(FSlateRoundedBoxBrush(
			FStyleColors::Primary, 4.0f,
		FStyleColors::Input, 1.f))
		.SetHovered(FSlateRoundedBoxBrush(
			FStyleColors::PrimaryHover, 4.0f,
			FStyleColors::Input, 1.f))
		.SetPressed(FSlateRoundedBoxBrush(
			FStyleColors::PrimaryPress, 4.0f,
		FStyleColors::Input, 1.f))
		.SetDisabled(FSlateRoundedBoxBrush(
			FStyleColors::Dropdown, 4.0f,
		FStyleColors::Recessed, 1.f))
		.SetNormalForeground(FStyleColors::ForegroundHover)
		.SetHoveredForeground(FStyleColors::ForegroundHover)
		.SetPressedForeground(FStyleColors::ForegroundHover)
		.SetDisabledForeground(FStyleColors::Foreground)
		.SetNormalPadding(FMargin(4, 2))
		.SetPressedPadding(FMargin(4, 2));

	Set("PrimaryButton", PrimaryButton);

	const FButtonStyle SimpleButton = FButtonStyle()
		.SetNormal(FSlateNoResource())
		.SetHovered(FSlateRoundedBoxBrush(FStyleColors::Dropdown, 4.0f))
		.SetPressed(FSlateRoundedBoxBrush(FStyleColors::Dropdown, 4.0f))
		.SetDisabled(FSlateNoResource())
		.SetNormalForeground(FStyleColors::Foreground)
		.SetHoveredForeground(FStyleColors::ForegroundHover)
		.SetPressedForeground(FStyleColors::ForegroundHover)
		.SetDisabledForeground(FStyleColors::Foreground)
		.SetNormalPadding(FMargin(4, 2))
		.SetPressedPadding(FMargin(4, 2));

	Set("SimpleButton", SimpleButton);

	FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FEasyInputEditorStyle::~FEasyInputEditorStyle()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

FEasyInputEditorStyle& FEasyInputEditorStyle::Get()
{
	static FEasyInputEditorStyle Inst;
	return Inst;
}
