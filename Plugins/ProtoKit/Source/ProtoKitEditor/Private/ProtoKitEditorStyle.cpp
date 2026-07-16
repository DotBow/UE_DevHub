// Copyright (C) Dreamer's Tail

#include "ProtoKitEditorStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"


FName FProtoKitEditorStyle::StyleName("ProtoKitEditorStyle");

FProtoKitEditorStyle::FProtoKitEditorStyle()
	: FSlateStyleSet(StyleName)
{
	FSlateStyleSet::SetContentRoot(
		IPluginManager::Get().FindPlugin("ProtoKit")->GetBaseDir() /
		TEXT("Resources/Icons"));
	FSlateStyleSet::SetCoreContentRoot(
		FPaths::EngineContentDir() / TEXT("Editor/Slate"));

	Set("ProtoKit",
		new IMAGE_BRUSH("ProtoKit", FVector2D(32, 32)));

	Set("ClassIcon.BaseMeshKit",
		new IMAGE_BRUSH("BaseMesh", FVector2D(32, 32)));

	Set("ProtoKit.Meshes.Box_1m_Corner",
		new IMAGE_BRUSH("Box_1m_Corner", FVector2D(32, 32)));
	Set("ProtoKit.Meshes.Box_1m_Center",
		new IMAGE_BRUSH("Box_1m_Center", FVector2D(32, 32)));

	Set("ProtoKit.Meshes.Cylinder_1m_8s",
		new IMAGE_BRUSH("Cylinder_1m_8s", FVector2D(32, 32)));
	Set("ProtoKit.Meshes.Cylinder_1m_16s",
		new IMAGE_BRUSH("Cylinder_1m_16s", FVector2D(32, 32)));
	Set("ProtoKit.Meshes.Cylinder_1m_32s",
		new IMAGE_BRUSH("Cylinder_1m_32s", FVector2D(32, 32)));

	Set("ProtoKit.Meshes.IcoSphere_1m_2d",
		new IMAGE_BRUSH("IcoSphere_1m_2d", FVector2D(32, 32)));

	Set("ProtoKit.Meshes.RoundCube_1m_2d",
		new IMAGE_BRUSH("RoundCube_1m_2d", FVector2D(32, 32)));
	Set("ProtoKit.Meshes.RoundCube_1m_4d",
		new IMAGE_BRUSH("RoundCube_1m_4d", FVector2D(32, 32)));

	Set("ProtoKit.Meshes.Sphere_1m_16s",
		new IMAGE_BRUSH("Sphere_1m_16s", FVector2D(32, 32)));
	Set("ProtoKit.Meshes.Sphere_1m_32s",
		new IMAGE_BRUSH("Sphere_1m_32s", FVector2D(32, 32)));

	FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FProtoKitEditorStyle::~FProtoKitEditorStyle()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

FProtoKitEditorStyle& FProtoKitEditorStyle::Get()
{
	static FProtoKitEditorStyle Inst;
	return Inst;
}
