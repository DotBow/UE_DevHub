// Copyright (C) Dreamer's Tail

#include "GamePulseSubsystem.h"

#include "GamePulse.h"
#include "GamePulseSettings.h"
#include "GamePulseWidget.h"
#include "TimerManager.h"


UGamePulseSubsystem* GamePulse;


TAutoConsoleVariable<bool> UGamePulseSubsystem::CVarGamePulseShow(
	TEXT("r.GamePulse.Show"),
	false,
	TEXT("Show Game Pulse widget"),
	ECVF_Cheat);


TAutoConsoleVariable<int32> UGamePulseSubsystem::CVarGamePulseAnchors(
	TEXT("r.GamePulse.Anchors"),
	static_cast<int32>(EGamePulseAnchors::TopLeft),
	TEXT("Set Game Pulse widget anchors\n")
	TEXT("0 - Top Left, 1 - Top Center, 2 - Top Right\n")
	TEXT("3 - Center Left, 4 - Center Center, 5 - Center Right\n")
	TEXT("6 - Bottom Left, 7 - Bottom Center, 8 - Bottom Right"),
	ECVF_Cheat);


void UGamePulseSubsystem::Initialize(
	FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GamePulseShowHandle =
		CVarGamePulseShow->OnChangedDelegate().AddStatic(
			&UGamePulseSubsystem::OnSetGamePulseShow);

	GamePulseAnchorsHandle =
		CVarGamePulseAnchors->OnChangedDelegate().AddStatic(
			&UGamePulseSubsystem::OnSetGamePulseAnchors);

	GamePulse = this;

	UE_LOG(LogGamePulse, Display,
		TEXT("Game Pulse Subsystem Initialized"))
}

void UGamePulseSubsystem::Deinitialize()
{
	if (GamePulseShowHandle.IsValid())
	{
		CVarGamePulseShow->OnChangedDelegate().Remove(
			GamePulseShowHandle);
	}

	if (GamePulseAnchorsHandle.IsValid())
	{
		CVarGamePulseAnchors->OnChangedDelegate().Remove(
			GamePulseAnchorsHandle);
	}

	Super::Deinitialize();

	UE_LOG(LogGamePulse, Display,
		TEXT("Game Pulse Subsystem Deinitialized"))
}

TStatId UGamePulseSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(
		UGamePulseSubsystem, STATGROUP_Tickables);
}

void UGamePulseSubsystem::Tick(
	const float DeltaTime)
{
	LastDeltaTime = DeltaTime;
}

bool UGamePulseSubsystem::IsTickable() const
{
	return CVarGamePulseShow->GetBool();
}

void UGamePulseSubsystem::OnWorldBeginPlay(
	UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	ShowGamePulse();
}

void UGamePulseSubsystem::OnSetGamePulseShow(
	IConsoleVariable* InVariable)
{
	if (GamePulse)
		GamePulse->ShowGamePulse();
}

void UGamePulseSubsystem::OnSetGamePulseAnchors(
	IConsoleVariable* InVariable)
{
	if (GamePulse)
		GamePulse->SetGamePulseAnchors();
}

void UGamePulseSubsystem::ShowGamePulse()
{
	if (CVarGamePulseShow->GetBool())
		AddGamePulseToViewport();
	else
		RemoveGamePulseFromViewport();
}

void UGamePulseSubsystem::SetGamePulseAnchors() const
{
	if (GamePulseWidget)
	{
		const EGamePulseAnchors NewAnchors =
			static_cast<EGamePulseAnchors>(
				CVarGamePulseAnchors->GetInt());
		GamePulseWidget->SetAnchors(NewAnchors);
		GamePulseWidget->SetAlignment(NewAnchors);
	}
}

void UGamePulseSubsystem::AddGamePulseToViewport()
{
	if (!GamePulseWidget)
	{
		if (UClass* GamePulseWidgetClass =
			UGamePulseSettings::Get().GetGamePulseWidgetClass())
		{
			GamePulseWidget = CreateWidget<UGamePulseWidget>(
				GetWorld(), GamePulseWidgetClass);

			if (GamePulseWidget)
			{
				GamePulseWidget->AddToViewport(255);

				GamePulseWidget->OnGetDeltaTime.BindUObject(this,
					&UGamePulseSubsystem::OnGetDeltaTime);

				GetWorld()->GetTimerManager().SetTimer(
					FPSTimerHandle, GamePulseWidget,
					&UGamePulseWidget::OnUpdateFPS,
					FPSTimerDelay, true);

				GetWorld()->GetTimerManager().SetTimer(
					StatTimerHandle, GamePulseWidget,
					&UGamePulseWidget::OnUpdateStats,
					StatTimerDelay, true);

				SetGamePulseAnchors();
			}
		}
	}
}

void UGamePulseSubsystem::RemoveGamePulseFromViewport()
{
	if (GamePulseWidget)
	{
		GamePulseWidget->RemoveFromParent();
		GamePulseWidget = nullptr;
	}

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}
