#pragma once
#include "KebabEngine.h"
#include <cstdio>


namespace cuc
{
	
	class KEBAB_API File
	{
	public:
		File(const char* path, const char* mode);
		~File();
	
		// Returns number of bytes read
		size_t ReadSync(char* buffer, size_t bufferSize);
		
		// Returns number of bytes written
		size_t WriteSync(const char* buffer, size_t bufferSize);
	
	private:
		FILE* m_pFile;
	};
}