#include "PCH.h"
#include "Atmosphere.h"


void AtmosphericData::Update(Atmosphere& atmosphere, F32 altitude)
{
	*this = atmosphere.GetAtmosphericData(altitude);
}

Atmosphere::Atmosphere()
{
	m_tempSoundOffset = 340.294f - 288.150f;

	m_tempSteps = {
		288.16f, // Starts at 0km   - troposphere
		216.66f, // Starts at 11km  - tropopause  
		216.66f, // Starts at 20km  - stratosphere
		270.65f, // Starts at 47km  - stratopause
		270.65f, // Starts at 54km  - mesosphere
		186.95f	 // Starts at 86km
	};

	m_tempStepAltitudes = {
		0,
		11000,
		20000,
		47000,
		54000,
		86000
	};
}

AtmosphericData Atmosphere::GetAtmosphericData(F32 altitude)
{
	AtmosphericData data;
	data.temperature = GetTemperature(altitude);
	data.soundSpeed = GetSoundSpeed(altitude);
	return data;
}

// In Kelvin
F32 Atmosphere::GetTemperature(F32 altitude)
{
	assert(altitude <= m_tempStepAltitudes[m_tempStepAltitudes.size() - 1]);

	// Find the interval @altitude is inside of 
	// [step - 1..step]
	U32 step = 1;
	for (; step < m_tempStepAltitudes.size(); step++)
	{
		if (m_tempStepAltitudes[step] > altitude)
		{
			break;
		}
	}

	// Rate of change is constant on each interval
	auto roc = (m_tempSteps[step] - m_tempSteps[step - 1]) / (m_tempStepAltitudes[step] - m_tempStepAltitudes[step - 1]);
	auto deltaAlt = altitude - m_tempStepAltitudes[step - 1];
	auto deltaTemp = roc * deltaAlt;
	return m_tempSteps[step - 1] + deltaTemp;
}

// In meters/second
F32 Atmosphere::GetSoundSpeed(F32 altitude)
{
	assert(altitude <= m_tempStepAltitudes[m_tempStepAltitudes.size() - 1]);
	return GetTemperature(altitude) + m_tempSoundOffset;
}