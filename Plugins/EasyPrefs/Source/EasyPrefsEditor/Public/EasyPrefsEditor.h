// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleManager.h"


class FEasyPrefsEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenuExtensions();
	static TSharedRef<SWidget> GenerateEasyPrefsOptionsMenu();
};
