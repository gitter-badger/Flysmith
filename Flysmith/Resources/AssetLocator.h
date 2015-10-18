#pragma once
#include <memory>


enum class AssetType
{
	NONE_ROOT, 
	SHADERS,
	FONTS,
	AIRFOILS,
	FUSELAGES, 
	WINGS
};

class AssetLocator
{
public:
	AssetLocator();
	const std::wstring& GetAssetDirectory(AssetType, bool bTrailingBackslash = true) const;
	const bool GetAssetPath(AssetType, const std::wstring& filename, std::wstring* outPath) const;

private:
	struct Impl;
	static std::unique_ptr<Impl> s_pImpl;
};