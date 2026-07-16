// Copyright (C) Dreamer's Tail

#pragma once
#include "Styling/SlateStyle.h"


class FEasyInputEditorStyle : public FSlateStyleSet
{
public:
	static FName StyleName;
	static FEasyInputEditorStyle& Get();

private:
	FEasyInputEditorStyle();
	virtual ~FEasyInputEditorStyle() override;
};
