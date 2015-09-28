#pragma once

// Mainly for rendering purposes.
// Even if it's a dummy 1:1 relationship, where objects with real-world dimensions are to be rendered, always use the conversion function.
// I.e. real-world dimensions are similar to magic numbers. 
// TODO: Make it prettier.
// TODO: Might use literals. 

// DX Units
// 1 meter = 0.01 DX units
// i.e. the far plane is at 1 km from the camera
const F32 kMeter = 1.f; 

inline F32 MetersToDXUnits(F32 meters)
{
	return meters * kMeter;
}

inline F32 DXUnitsToMeters(F32 dxUnits)
{
	return dxUnits * (1.0f / kMeter);
}