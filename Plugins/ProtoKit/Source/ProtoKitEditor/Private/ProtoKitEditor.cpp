// Copyright (C) Dreamer's Tail

#include "ProtoKitEditor.h"

#include "ProtoKitEditorStyle.h"
#include "ProtoKitPlacement.h"


#define LOCTEXT_NAMESPACE "ProtoKitEditorModule"

void FProtoKitEditorModule::StartupModule()
{
	FProtoKitEditorStyle::Get();
	FProtoKitPlacement::RegisterPlacement();
}

void FProtoKitEditorModule::ShutdownModule()
{
	FProtoKitPlacement::UnregisterPlacement();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProtoKitEditorModule, ProtoKitEditor)
