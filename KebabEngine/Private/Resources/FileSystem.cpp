#include "PCH.h"
#include "FileSystem.h"
using namespace cuc;


struct FileSystem::Impl
{
	Impl();

	std::wstring m_exePath;
};
std::unique_ptr<FileSystem::Impl> FileSystem::s_pImpl = nullptr;

FileSystem::FileSystem()
{
	if (s_pImpl == nullptr)
	{
		s_pImpl = std::make_unique<Impl>();
	}
}

const wchar_t* FileSystem::GetExePath() const
{
	return s_pImpl->m_exePath.c_str();
}

FileSystem::Impl::Impl()
{
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	m_exePath = path;
}

void FileSystem::RemoveLastNameFromPath(std::wstring* path, bool bLeaveTrailingBackslash)
{
	assert(path != nullptr);

	size_t searchStartPos = path->size() - 1;

	// If there is a trailing backslash, search for the backslash before it
	// i.e. for D:\NotPorn\, search for the first backslash's position(before NotPorn) 
	if (path->at(path->size() - 1) == '\\')
		searchStartPos--;

	auto backslashPos = path->find_last_of('\\', searchStartPos);
	if (backslashPos != std::wstring::npos)
		*path = bLeaveTrailingBackslash ? path->substr(0, backslashPos + 1) 
									    : path->substr(0, backslashPos);
}

bool FileSystem::FileExists(const wchar_t* path)
{
	DWORD dwAttrib = GetFileAttributes(path);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
