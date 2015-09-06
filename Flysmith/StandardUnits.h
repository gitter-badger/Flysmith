#pragma once

// Mainly for rendering purposes.
// Even if it's a dummy 1:1 relationship, where objects with real-world dimensions are to be rendered, always use the conversion function.
// I.e. real-world dimensions are similar to magic numbers. 
// TODO: Make it prettier.
// TODO: Might use literals. 

// DX Units
// 1 meter = 1 DX unit
// i.e. the far plane is at 1 km from the camera
const int kMeter = 1.0f; 

inline float MetersToDXUnits(float meters)
{
	return meters * kMeter;
}

inline float DXUnitsToMeters(float dxUnits)
{
	return dxUnits * (1.0f / kMeter);
}