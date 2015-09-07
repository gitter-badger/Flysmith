#include "PCH.h"
#include "AssetLocator.h"
#include "FileSystem.h"


struct AssetLocator::Impl
{
	Impl();

	std::map<AssetType, std::wstring> assetDirectories;
};
std::unique_ptr<AssetLocator::Impl> AssetLocator::s_pImpl = nullptr;

AssetLocator::AssetLocator()
{
	if (s_pImpl == nullptr)
	{
		s_pImpl = std::make_unique<Impl>();
	}
}

const std::wstring& AssetLocator::GetAssetDirectory(AssetType assetType, bool bTrailingBackslash) const
{
	if (!bTrailingBackslash)
	{
		return std::move(s_pImpl->assetDirectories[assetType].substr(0, s_pImpl->assetDirectories[assetType].size() - 1));
	}

	return s_pImpl->assetDirectories[assetType];
}

const bool AssetLocator::GetAssetPath(AssetType assetType, const std::wstring& filename, std::wstring* outPath) const
{
	*outPath = GetAssetDirectory(assetType) + filename;
	return FileSystem::FileExists(outPath->c_str());
}

AssetLocator::Impl::Impl()
{
	FileSystem fs;
	std::wstring rootPath(fs.GetExePath());

#ifdef _DEBUG
	// Root is the Solution folder

	// Remove executable name
	FileSystem::RemoveLastNameFromPath(&rootPath, true);

	// Remove build configuration folder
	FileSystem::RemoveLastNameFromPath(&rootPath, true);

	// Remove Bin folder
	FileSystem::RemoveLastNameFromPath(&rootPath, true);
#else /* REDISTRIBUTING */
	// Root is the executable folder

	// Remove executable name and trailing slash
	rootPath = rootPath.substr(0, rootPath.find_last_of(L"\\") + 1);
#endif

	assetDirectories[AssetType::NONE_ROOT] = rootPath + L"Assets\\";
	assetDirectories[AssetType::SHADERS] = assetDirectories[AssetType::NONE_ROOT] + L"Shaders\\";
	assetDirectories[AssetType::FONTS] = assetDirectories[AssetType::NONE_ROOT] + L"Fonts\\";
	assetDirectories[AssetType::AIRFOILS] = assetDirectories[AssetType::NONE_ROOT] + L"Airfoils\\";
	assetDirectories[AssetType::FUSELAGES] = assetDirectories[AssetType::NONE_ROOT] + L"Fuselages\\";
}