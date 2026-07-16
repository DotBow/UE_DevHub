// Copyright (C) Dreamer's Tail

#pragma once

#include "Components/ActorComponent.h"
#include "EasyInputComponent.generated.h"

struct FUIActionBindingHandle;


UCLASS(Within=PlayerController, ClassGroup=(Input), meta=(BlueprintSpawnableComponent))
class EASYINPUT_API UEasyInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEasyInputComponent();

	static void RegisterWidgetInputs(
		class UCommonUserWidget* Widget);

	virtual TObjectPtr<class UEasyInputBindings> GetActiveInputBindings() const;

	UFUNCTION(BlueprintCallable)
	void SetActiveInputBindingIndex(
		const int32 InActiveInputIndex,
		UObject* InObject = nullptr,
		const bool bSetInputBindings = true);

protected:
	void RegisterWidgetInputs_Imp(
		UCommonUserWidget* Widget) const;

private:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	int32 ActiveInputIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TArray<TObjectPtr<UEasyInputBindings>> InputBindings;

public:
	virtual void BeginPlay() override;
	void SetInputBindings(
		UObject* InObject = nullptr) const;

protected:
	void SetInputBindings(
		const TObjectPtr<APlayerController>& PlayerController,
		UObject* InObject) const;
};
