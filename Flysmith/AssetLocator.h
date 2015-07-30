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
	const std::wstring& GetAssetPath(const AssetDirectory, const wchar_t* filename) const;
	const std::wstring& GetAssetPath(const AssetDirectory, const std::wstring& filename) const;

private:
	struct Impl;
	static std::unique_ptr<Impl> s_pImpl;
};