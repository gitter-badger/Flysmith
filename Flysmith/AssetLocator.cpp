#include "PCH.h"
#include "AssetLocator.h"
#include "FileSystem.h"


struct AssetLocator::Impl
{
	Impl();

	std::map<AssetDirectory, std::wstring> assetDirectories;
};
std::unique_ptr<AssetLocator::Impl> AssetLocator::s_pImpl = nullptr;

AssetLocator::AssetLocator()
{
	if (s_pImpl == nullptr)
	{
		s_pImpl = std::make_unique<Impl>();
	}
}

const std::wstring& AssetLocator::GetAssetDirectory(const AssetDirectory assetType, bool bTrailingBackslash) const
{
	if (!bTrailingBackslash)
	{
		return std::move(s_pImpl->assetDirectories[assetType].substr(0, s_pImpl->assetDirectories[assetType].size() - 1));
	}

	return s_pImpl->assetDirectories[assetType];
}

const bool AssetLocator::GetAssetPath(const AssetDirectory assetType, const std::wstring& filename, std::wstring* outPath) const
{
	*outPath = GetAssetDirectory(assetType) + filename;
	return FileSystem::FileExists(outPath->c_str());
}

AssetLocator::Impl::Impl()
{
	FileSystem fs;
	std::wstring projectRootPath(fs.GetExePath());

	// For now, place all assets in a single folder in the solution directory instead of making copies for each build configuration.
	// Configuration folder
	FileSystem::RemoveLastNameFromPath(&projectRootPath, true);
	// Bin folder
	FileSystem::RemoveLastNameFromPath(&projectRootPath, true);
	// Solution folder
	FileSystem::RemoveLastNameFromPath(&projectRootPath, true);

	assetDirectories[AssetDirectory::ROOT] = projectRootPath + L"Assets\\";
	assetDirectories[AssetDirectory::SHADERS] = assetDirectories[AssetDirectory::ROOT] + L"Shaders\\";
	assetDirectories[AssetDirectory::FONTS] = assetDirectories[AssetDirectory::ROOT] + L"Fonts\\";
	assetDirectories[AssetDirectory::AIRFOILS] = assetDirectories[AssetDirectory::ROOT] + L"Airfoils\\";
}