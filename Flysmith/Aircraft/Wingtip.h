#pragma once


struct Mesh;
struct DirectX::XMFLOAT3;

// Array of vertex/point positions.
// Not using 'vector', even though it would be more appropriate, to avoid confusion.
using VertexArray = std::vector<DirectX::XMFLOAT3>;

class Wingtip
{
public:
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

	// For now, it has to match the wing's airfoil at the tip.
	VertexArray tipAirfoil;
	
public:
	Wingtip();
	Mesh Generate();

private:
	void GenerateCutoff(Mesh& meshOut);
};