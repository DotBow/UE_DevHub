// Copyright (C) Dreamer's Tail

#pragma once
#include "Styling/SlateStyle.h"


class FProtoKitEditorStyle : public FSlateStyleSet
{
public:
	static FName StyleName;
	static FProtoKitEditorStyle& Get();

private:
	FProtoKitEditorStyle();
	virtual ~FProtoKitEditorStyle() override;
};
