#pragma once
#include "PCH.h"


class Atmosphere;

struct AtmosphericData
{
	F32 temperature; // In Kelvin
	F32 soundSpeed;  // In meters/second

	void Update(Atmosphere&, F32 altitude);
};

// TODO: Don't hardcode values
// TODO: Precompute lots of the stuff getting recalculated on each query 
class Atmosphere
{
public:
	Atmosphere();

	// Altitude in meters
	// Get all state at some altitude instead of querying each individual property. 
	AtmosphericData GetAtmosphericData(F32 altitude);

	// Temperature in Kelvin
	// Altitude in meters
	F32 GetTemperature(F32 altitude);

	// Sound speed in meters/second
	// Altitude in meters
	F32 GetSoundSpeed(F32 altitude);

private:
	std::vector<F32> m_tempSteps;
	std::vector<F32> m_tempStepAltitudes;

	// Rates of change for temperature and speed of sound are equal.
	// Will offset temperature data instead of duplicating it for speed of sound.
	F32 m_tempSoundOffset;
};