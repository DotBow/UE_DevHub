// Copyright (C) Dreamer's Tail

#pragma once

#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"
#include "Factories/DataAssetFactory.h"
#include "EasyInputBindings.h"
#include "EasyInputBindingsAsset.generated.h"


class SEasyInputDetails : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEasyInputDetails) {}
	SLATE_END_ARGS();

	void Construct(
		const FArguments& InArgs,
		UEasyInputBindings* InInputBindings);

protected:
	UEasyInputBindings* InputBindings = nullptr;
	TSharedPtr<IDetailsView> DetailsView;

	void Draw();
};


class SEasyInputCategories : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEasyInputCategories) {}
	SLATE_END_ARGS();

	void Construct(
		const FArguments& InArgs,
		UEasyInputBindings* InInputBindings);

protected:
	UEasyInputBindings* InputBindings = nullptr;

	void Draw();
};


class SEasyInputBindingType : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEasyInputBindingType) {}
	SLATE_ARGUMENT(EEasyInputBindingType, BindingType)
	SLATE_ARGUMENT(FText, Label)
	SLATE_ATTRIBUTE(const FSlateBrush*, Image)
SLATE_END_ARGS();

	void Construct(
		const FArguments& InArgs,
		UEasyInputBindings* InInputBindings);

private:
	EEasyInputBindingType BindingType = EEasyInputBindingType::Action;
	UEasyInputBindings* InputBindings = nullptr;

	FReply OnClicked() const;
};


class FEasyInputBindingsToolkit final : public FAssetEditorToolkit
{
public:
	void InitEditor(
		const TArray<UObject*>& InObjects);
	virtual void OnClose() override;

protected:
	virtual void RegisterTabSpawners(
		const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(
		const TSharedRef<FTabManager>& TabManager) override;
	void AddToolbarExtension(
		FToolBarBuilder& ToolbarBuilder) const;

	virtual FName GetToolkitFName() const override
	{
		return "EasyInputBindingsEditor";
	}

	virtual FText GetBaseToolkitName() const override
	{
		return INVTEXT("Easy Input Bindings Editor");
	}

	virtual FString GetWorldCentricTabPrefix() const override
	{
		return "Easy Input Bindings Editor ";
	}

	virtual FLinearColor GetWorldCentricTabColorScale() const override
	{
		return {};
	}

	UEasyInputBindings* InputBindings = nullptr;
	static const FName DetailsTabId;

	TSharedPtr<SEasyInputDetails> Details;

public:
	UEasyInputBindings* GetInputBindings() const
	{
		return InputBindings;
	}
};


UCLASS()
class EASYINPUTEDITOR_API UEasyInputBindingsFactory : public UDataAssetFactory
{
	GENERATED_BODY()

	UEasyInputBindingsFactory();

	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(
		UClass* Class,
		UObject* InParent,
		FName Name,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn) override;
};


class EASYINPUTEDITOR_API FEasyInputBindingsActions : public FAssetTypeActions_Base
{
public:
	FEasyInputBindingsActions(EAssetTypeCategories::Type AssetCategory)
		: AssetCategory(AssetCategory) {}

	virtual UClass* GetSupportedClass() const override
	{
		return UEasyInputBindings::StaticClass();
	}

	virtual FText GetName() const override
	{
		return INVTEXT("Input Bindings");
	}

	virtual FColor GetTypeColor() const override
	{
		return FColor::FromHex("ffb627");
	}

	virtual uint32 GetCategories() override
	{
		return AssetCategory;
	}

	virtual void OpenAssetEditor(
		const TArray<UObject*>& InObjects,
		TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

private:
	EAssetTypeCategories::Type AssetCategory =
		EAssetTypeCategories::Misc;
};
