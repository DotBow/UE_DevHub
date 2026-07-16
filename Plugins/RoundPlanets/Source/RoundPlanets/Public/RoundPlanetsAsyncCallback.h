// Copyright (C) Dreamer's Tail

#pragma once


struct FGravityAttractorData
{
	FVector Location;
	double MassDotG;
};

struct ROUNDPLANETS_API FCustomGravityAsyncInput : public Chaos::FSimCallbackInput
{
	TArray<FGravityAttractorData> GravityAttractorsData;

	void Reset()
	{
		GravityAttractorsData.Empty();
	}
};


class ROUNDPLANETS_API FRoundPlanetsAsyncCallback : public Chaos::TSimCallbackObject<
	FCustomGravityAsyncInput, // A custom Input class to convey data from the DT to the PT
	Chaos::FSimCallbackNoOutput, // No need to output anything here
	Chaos::ESimCallbackOptions::PreIntegrate |Chaos::ESimCallbackOptions::Presimulate> // The hooks we want to register into -> Implement the corresponding virtual methods below
{
public:
	FRoundPlanetsAsyncCallback();
	virtual ~FRoundPlanetsAsyncCallback() override;

	virtual void OnPreSimulate_Internal() override;
	virtual void OnPreIntegrate_Internal() override;

protected:
	static double GravitationalConstant;  
};
