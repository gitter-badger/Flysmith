#include "File.h"
#include <cstdio>
#include <cassert>
using namespace cuc;


File::File(const char* path, const char* mode)
	: m_pFile(nullptr)
{
	assert(path != nullptr);
	assert(mode != nullptr);

	auto error = fopen_s(&m_pFile, path, mode);
	if(error)
	{
		// TODO: Log error
	}
}

File::~File()
{
	fclose(m_pFile);
}

size_t File::ReadSync(char* buffer, size_t bufferSize)
{
	assert(buffer != nullptr);
	assert(bufferSize > 0);
	
	size_t bytesRead = fread(buffer, 1, bufferSize, m_pFile);
		
	auto readError = ferror(m_pFile);
	if(readError)
	{
		bytesRead = 0;
		// TODO: Log error
	}
	
	return bytesRead;
}
		
size_t File::WriteSync(const char* buffer, size_t bufferSize)
{
	assert(buffer != nullptr);
	assert(bufferSize > 0);
	
	size_t bytesWritten = fwrite(buffer, 1, bufferSize, m_pFile);
		
	auto readError = ferror(m_pFile);
	if(readError)
	{
		bytesWritten = 0;
		// TODO: Log error
	}
	
	return bytesWritten;
}