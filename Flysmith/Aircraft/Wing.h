#pragma once
#include "Wingtip.h"

struct Mesh;
struct DirectX::XMFLOAT3;

// Array of vertex/point positions.
// Not using 'vector', even though it would be more appropriate, to avoid confusion.
using VertexArray = std::vector<DirectX::XMFLOAT3>;

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

	WingRing();
};

struct WingSection
{
	// In degrees
	// Relative to root
	F32 sweep; 
	
	// In degrees
	// Relative to root 
	F32 dihedral;

	WingSection();
};

struct Wing
{
	// Doesn't include full path and extension
	// i.e. NACA2412 and not D:\Flysmith\Assets\NACA2412.dat
	std::wstring airfoilFile;

	std::vector<WingRing> rings;
	
	// In meters
	// Length from root to tip(i.e. not wingspan)
	F32 length;

	Wingtip wingtip;

	std::vector<WingSection> sections;

	Wing();
	Mesh GenerateMesh();

private:
	void CheckConfigurationValidity();
	void GenerateAirfoils();
	void PlaceRingsAlongWing();
	void ScaleSectionsByChord();
	void ApplySweeps();
	void ApplyDihedrals();
	void GenerateMeshVertsIndices(Mesh&);

	std::vector<VertexArray> airfoils;
};