// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleManager.h"


class FEasyInputEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<class IAssetTypeActions> EasyInputBindingsActions;
};
