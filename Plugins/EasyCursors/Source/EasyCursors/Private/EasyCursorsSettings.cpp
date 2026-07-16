// Copyright (C) Dreamer's Tail

#include "EasyCursorsSettings.h"

#include "ImageUtils.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/UserInterfaceSettings.h"
#include "Kismet/KismetRenderingLibrary.h"


UEasyCursorsSettings::UEasyCursorsSettings()
{
	const UEnum* MouseCursorTypeEnum = StaticEnum<EMouseCursor::Type>();

	for (int32 Idx = 0; Idx < MouseCursorTypeEnum->NumEnums() - 1; Idx++)
	{
		const EMouseCursor::Type MouseCursorType = static_cast<EMouseCursor::Type>(
			MouseCursorTypeEnum->GetValueByIndex(Idx));

		if (MouseCursorType != EMouseCursor::None)
			HardwareCursors.Add(MouseCursorType, FSoftClassPath(""));
	}
}

UEasyCursorsSettings* UEasyCursorsSettings::Get()
{
	return GetMutableDefault<UEasyCursorsSettings>();
}

const TMap<TEnumAsByte<EMouseCursor::Type>, FSoftObjectPath>& UEasyCursorsSettings::GetHardwareCursors() const
{
	return HardwareCursors;
}

FSoftObjectPath UEasyCursorsSettings::GetHardwareCursor(
	const TEnumAsByte<EMouseCursor::Type> MouseCursorType) const
{
	if (MouseCursorType != EMouseCursor::None)
	{
		if (HardwareCursors.Contains(MouseCursorType))
			return HardwareCursors[MouseCursorType];
	}

	return FSoftObjectPath();
}

void UEasyCursorsSettings::SetHardwareCursor(
	const TEnumAsByte<EMouseCursor::Type> MouseCursorType,
	const FSoftObjectPath& ObjectPath)
{
	if (MouseCursorType != EMouseCursor::None)
		HardwareCursors.Add(MouseCursorType, ObjectPath);
}

FString UEasyCursorsSettings::GetMouseCursorTypeName(
	const TEnumAsByte<EMouseCursor::Type> MouseCursorType)
{
	FString MouseCursorTypeName =
		*UEnum::GetValueAsName(MouseCursorType).ToString();
	MouseCursorTypeName.RemoveFromStart(TEXT("EMouseCursor::"));

	return MouseCursorTypeName;
}

FIntVector2 UEasyCursorsSettings::GetCursorSize(
	const ECursorSize InSize)
{
	if (InSize == ECursorSize::x24)
		return FIntVector2(24);
	if (InSize == ECursorSize::x32)
		return FIntVector2(32);
	if (InSize == ECursorSize::x48)
		return FIntVector2(48);

	return FIntVector2(0);
}

void UEasyCursorsSettings::RenderCursor(
	UMaterialInterface* InCursorMaterial,
	const EMouseCursor::Type InMouseCursor,
	const ECursorSize InSize)
{
	UTextureRenderTarget2D* RenderTarget2D =
		NewObject<UTextureRenderTarget2D>(
		UTextureRenderTarget2D::StaticClass(),
		NAME_None,
		RF_Transient);

	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	const FIntVector2 CursorSize = GetCursorSize(InSize);
	RenderTarget2D->InitAutoFormat(CursorSize.X, CursorSize.Y);
	RenderTarget2D->ClearColor = FLinearColor(0, 0, 0, 0);
	RenderTarget2D->UpdateResourceImmediate(true);

	UWorld* World = GEditor->GetEditorWorldContext().World();
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(
		World, RenderTarget2D, InCursorMaterial);

	FImage Image;
	FImageUtils::GetRenderTargetImage(
		RenderTarget2D, Image);

	FColor* Colors = reinterpret_cast<FColor*>(Image.RawData.GetData());
	const int32 NumColors = Image.RawData.Num() / 4;

	for(int32 ColorIndex = 0; ColorIndex < NumColors; ColorIndex++)
	{
		Colors[ColorIndex].A = 255 - Colors[ColorIndex].A;
	}

	const FString Filename = GetMouseCursorTypeName(InMouseCursor);
	const FString CursorPath =
		FString::Format(TEXT("{0}/{1}/{2}.png"), {
			FPaths::ProjectContentDir(),
			TEXT("Cursors"), Filename });
	FImageUtils::SaveImageByExtension(
		*CursorPath, Image);

	UUserInterfaceSettings* UISettings =
		GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());
	FHardwareCursorReference HardwareCursorReference;
	HardwareCursorReference.CursorPath = FName(
		FString::Format(TEXT("{0}/{1}"),
		{ TEXT("Cursors"), Filename }));
	UISettings->HardwareCursors.Add(
		InMouseCursor, HardwareCursorReference);
}

void UEasyCursorsSettings::PostEditChangeChainProperty(
	FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() ==
		GetHardwareCursorsPropertyName())
	{
		const int32 ArrayIdx = PropertyChangedEvent.GetArrayIndex(
			GetHardwareCursorsPropertyName().ToString());

		if (ArrayIdx != INDEX_NONE)
		{
			TArray<TEnumAsByte<EMouseCursor::Type>> OutKeys;
			HardwareCursors.GetKeys(OutKeys);

			if (ArrayIdx < OutKeys.Num())
			{
				const TEnumAsByte<EMouseCursor::Type> MouseCursorType =
					OutKeys[ArrayIdx];

				if (!HardwareCursors[MouseCursorType].IsNull())
				{
					if (UMaterialInterface* MaterialInterface = Cast<UMaterialInterface>(
						HardwareCursors[MouseCursorType].TryLoad()))
					{
						RenderCursor(MaterialInterface, MouseCursorType);
					}
				}
			}
		}
	}
}
