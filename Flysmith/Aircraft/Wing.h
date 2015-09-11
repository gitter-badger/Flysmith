#pragma once


struct Mesh;
struct DirectX::XMFLOAT3;

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

struct Wingtip
{
	enum Type
	{
		CUTOFF,
		ROUNDED,
		SHARP,
		HOERNER,
		DROOPED,
		ENDPLATE,
		WINGLET
	} type;

	Wingtip();
	// TODO: Less retarded explanation
	// @tipVertexBegin - offset index in the list of vertices already generated for the wing, up to the tip
	// i.e. if the wing uses only one airfoil plotted with 10 points and the wing has only one section(2 rings - for root and tip),
	//      tipVertexBegin would be 0 + (numRings - 1) * pointsPerAirfoil = 0 + 1 * 10 = 10 
	void Generate(Mesh&, std::vector<DirectX::XMFLOAT3>& tipAirfoil, U32 tipVertexBegin);
	void GenerateCutoff(Mesh&, std::vector<DirectX::XMFLOAT3>& tipAirfoil, U32 tipVertexBegin);
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

public:
	Wing();
	Mesh GenerateMesh();

private:
	void CheckConfigurationValidity();
	void GenerateAirfoils(std::vector<std::vector<DirectX::XMFLOAT3>>& airfoils);
	void PlaceRingsAlongWing(std::vector<std::vector<DirectX::XMFLOAT3>>& airfoils);
	void ScaleSectionsByChord(std::vector<std::vector<DirectX::XMFLOAT3>>& airfoils);
	void ApplySweeps(std::vector<std::vector<DirectX::XMFLOAT3>>& airfoils);
	void ApplyDihedrals(std::vector<std::vector<DirectX::XMFLOAT3>>& airfoils);
	void GenerateMeshVertsIndices(Mesh&, std::vector<std::vector<DirectX::XMFLOAT3>>& airfoils);
};