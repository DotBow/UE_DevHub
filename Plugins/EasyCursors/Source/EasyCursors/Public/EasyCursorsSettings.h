// Copyright (C) Dreamer's Tail

#pragma once
#include "EasyCursorsSettings.generated.h"


UENUM(NotBlueprintable, NotBlueprintType)
enum class ECursorSize : uint8
{
	x24 = 0 << 0 UMETA(DisplayName = "24x24"),
	x32 = 1 << 0 UMETA(DisplayName = "32x32"),
	x48 = 2 << 0 UMETA(DisplayName = "48x48"),
};


UCLASS(Config=Editor, DefaultConfig, MinimalAPI)
class UEasyCursorsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UEasyCursorsSettings();

private:
	UPROPERTY(Config, EditAnywhere)
	TMap<TEnumAsByte<EMouseCursor::Type>, FSoftObjectPath> HardwareCursors;

public:
	static FName GetHardwareCursorsPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(
			UEasyCursorsSettings, HardwareCursors);
	}

	static UEasyCursorsSettings* Get();

	const TMap<TEnumAsByte<EMouseCursor::Type>, FSoftObjectPath>& GetHardwareCursors() const;
	FSoftObjectPath GetHardwareCursor(
		TEnumAsByte<EMouseCursor::Type> MouseCursorType) const;
	void SetHardwareCursor(
		TEnumAsByte<EMouseCursor::Type> MouseCursorType,
		const FSoftObjectPath& ObjectPath);

	static FString GetMouseCursorTypeName(
		TEnumAsByte<EMouseCursor::Type> MouseCursorType);

	static FIntVector2 GetCursorSize(
		const ECursorSize InSize);
	static void RenderCursor(
		UMaterialInterface* InCursorMaterial,
		EMouseCursor::Type InMouseCursor,
		const ECursorSize InSize = ECursorSize::x24);

private:
	virtual void PostEditChangeChainProperty(
		FPropertyChangedChainEvent& PropertyChangedEvent) override;
};
