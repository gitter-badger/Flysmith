#include "PCH.h"
#include "AssetLocator.h"
#include "FileSystem.h"
#include <string>


struct AssetLocator::Impl
{
	Impl();

	std::wstring m_assetsPath;
	std::wstring m_shadersPath;
	std::wstring m_fontsPath;
	std::wstring m_airfoilsPath;

};
std::unique_ptr<AssetLocator::Impl> AssetLocator::s_pImpl = nullptr;

AssetLocator::AssetLocator()
{
	if (s_pImpl == nullptr)
	{
		s_pImpl = std::make_unique<Impl>();
	}
}

const wchar_t* AssetLocator::GetAssetsPath() const
{
	return s_pImpl->m_assetsPath.c_str();
}

const wchar_t* AssetLocator::GetShadersPath() const
{
	return s_pImpl->m_shadersPath.c_str();
}

const wchar_t* AssetLocator::GetFontsPath() const
{
	return s_pImpl->m_fontsPath.c_str();
}

const wchar_t* AssetLocator::GetAirfoilsPath() const
{
	return s_pImpl->m_airfoilsPath.c_str();
}

AssetLocator::Impl::Impl()
{
	cuc::FileSystem fs;
	std::wstring projectRootPath(fs.GetExePath());

	// For now, place all assets in a single folder in the solution directory instead of making copies for each build configuration.
	// Configuration folder
	fs.RemoveLastNameFromPath(&projectRootPath);
	// Bin folder
	fs.RemoveLastNameFromPath(&projectRootPath);
	// Solution folder
	fs.RemoveLastNameFromPath(&projectRootPath);

	m_assetsPath = projectRootPath + L"Assets\\";
	m_shadersPath = m_assetsPath + L"Shaders\\";
	m_fontsPath = m_assetsPath + L"Fonts\\";
	m_airfoilsPath = m_assetsPath + L"Airfoils\\";
}