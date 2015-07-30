#pragma once
#include <memory>


enum class AssetDirectory
{
	ROOT,
	SHADERS,
	FONTS,
	AIRFOILS
};

class AssetLocator
{
public:
	AssetLocator();
	const std::wstring& GetAssetDirectory(const AssetDirectory, bool bTrailingBackslash = true) const;
	const bool GetAssetPath(const AssetDirectory, const std::wstring& filename, std::wstring* outPath) const;

private:
	struct Impl;
	static std::unique_ptr<Impl> s_pImpl;
};