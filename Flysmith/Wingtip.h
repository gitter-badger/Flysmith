#pragma once


struct Mesh;
struct DirectX::XMFLOAT3;


struct Wingtip
{
	// In meters 
	// Matches the wing's last ring chord
	F32 chord;

	// Doesn't include full path and extension
	// i.e. NACA2412 and not D:\Flysmith\Assets\NACA2412.dat
	// Matches the airfoil on the wing's last ring
	std::wstring airfoilFile;

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

	Mesh GenerateMesh();

	Wingtip();

private:
	void GenerateCutoff(Mesh&, std::vector<DirectX::XMFLOAT3>& airfoil);
};