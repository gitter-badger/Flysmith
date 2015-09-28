#pragma once


struct Mesh;
struct DirectX::XMFLOAT3;

// Array of vertex/point positions.
// Not using 'vector', even though it would be more appropriate, to avoid confusion.
using VertexArray = std::vector<DirectX::XMFLOAT3>;

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
	void Generate(Mesh&, VertexArray& tipAirfoil, U32 tipVertexBegin);
	void GenerateCutoff(Mesh&, VertexArray& tipAirfoil, U32 tipVertexBegin);
};