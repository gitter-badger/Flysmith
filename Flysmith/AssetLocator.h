#pragma once
#include <memory>


class AssetLocator
{
public:
	AssetLocator();
	const wchar_t* GetAssetsPath() const;
	const wchar_t* GetShadersPath() const;
	const wchar_t* GetFontsPath() const;
	const wchar_t* GetAirfoilsPath() const;

private:
	struct Impl;
	static std::unique_ptr<Impl> s_pImpl;
};