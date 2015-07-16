#include "PCH.h"
#include "FileSystem.h"



struct cuc::FileSystem::Impl
{
	Impl();
	char m_exePath[MAX_PATH];
	char m_assetsPath[MAX_PATH];
	char m_shadersPath[MAX_PATH];
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
	return s_pImpl->m_exePath;
}

const char * cuc::FileSystem::GetAssetsPath()
{
	return s_pImpl->m_assetsPath;
}

const char* cuc::FileSystem::GetShadersPath()
{
	return s_pImpl->m_shadersPath;
}

#pragma warning(push)
#pragma warning(disable:4996)
cuc::FileSystem::Impl::Impl()
{
	GetModuleFileNameA(NULL, m_exePath, MAX_PATH);

	auto backslashPos = strrchr(m_exePath, '\\') + 1;
	*backslashPos = '\0';

	strcpy(m_assetsPath, m_exePath);
	strcat(m_assetsPath, "Assets\\");

	strcpy(m_shadersPath, m_assetsPath);
	strcat(m_shadersPath, "Shaders\\");
}
#pragma warning(pop)