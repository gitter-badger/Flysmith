#include "PCH.h"
#include "FileSystem.h"



struct cuc::FileSystem::Impl
{
	Impl();
	void RemoveLastNameFromPath(std::string& path, bool bLeaveTrailingBackslash = true);

	std::string m_exePath;
	std::string m_assetsPath;
	std::string m_shadersPath;
	std::string m_fontsPath;
};
std::unique_ptr<cuc::FileSystem::Impl> cuc::FileSystem::s_pImpl = nullptr;


cuc::FileSystem::FileSystem()
{
	if (s_pImpl == nullptr)
	{
		s_pImpl = std::make_unique<Impl>();
	}
}

const char* cuc::FileSystem::GetExePath()
{
	return s_pImpl->m_exePath.c_str();
}

const char* cuc::FileSystem::GetAssetsPath()
{
	return s_pImpl->m_assetsPath.c_str();
}

const char* cuc::FileSystem::GetShadersPath()
{
	return s_pImpl->m_shadersPath.c_str();
}

const char* cuc::FileSystem::GetFontsPath()
{
	return s_pImpl->m_fontsPath.c_str();
}

cuc::FileSystem::Impl::Impl()
{
	char cPath[MAX_PATH];
	GetModuleFileNameA(NULL, cPath, MAX_PATH);
	m_exePath = cPath;

	// For now, place all assets in a single folder in the solution directory instead of making copies for each build configuration.
	// Configuration folder
	RemoveLastNameFromPath(m_exePath);
	// Bin folder
	RemoveLastNameFromPath(m_exePath);
	// Solution folder
	RemoveLastNameFromPath(m_exePath);
	
	m_assetsPath = m_exePath + "Assets\\";
	m_shadersPath = m_assetsPath + "Shaders\\";
	m_fontsPath   = m_assetsPath + "Fonts\\";
}

void cuc::FileSystem::Impl::RemoveLastNameFromPath(std::string& path, bool bLeaveTrailingBackslash)
{
	size_t searchStartPos = path.size() - 1;

	// If there is a trailing backslash, search for the backslash before it
	// i.e. for D:\NotPorn\, search for the first backslash's position(before NotPorn) 
	if (path[path.size() - 1] == '\\')
		searchStartPos--;

	auto backslashPos = path.find_last_of('\\', searchStartPos);
	if (backslashPos != std::string::npos)
		path = bLeaveTrailingBackslash ? path.substr(0, backslashPos + 1) 
									   : path.substr(0, backslashPos);
}