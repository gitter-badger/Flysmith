#pragma once


struct Mesh;

// Rings divide the wing into sections(2 rings delimit such a section), each with constant cross section profile 
// and constant rate of change in parameters
struct WingRing
{
	// In percentage from 0 to 1, measured from root
	// i.e. the root ring is at 0 
	//      some ring in the middle of the wing is at 0.5
	//      the tip ring is at 1(not including tip device)
	F32 locationOnWing;

	// In meters
	// Linearly interpolated between rings
	F32 chord;

	// In degrees
	// Linearly interpolated between rings
	F32 incidenceAngle;
};

struct Wing
{
	// Doesn't include full path and extension
	// i.e. NACA2412 and not D:\Flysmith\Assets\NACA2412.dat
	// TODO: Cache airfoil data and require the desired airfoil's id in the cache 
	//       Better, generate airfoil data 
	std::wstring airfoilFile;
	std::vector<WingRing> rings;

	Mesh GenerateMesh();
};