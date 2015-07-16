#include "PCH.h"
#include "FileReader.h"
using namespace cuc;


FileReader::FileReader()
{
	m_thread = std::thread(&FileReader::ReadThread, this);
}

FileReader::~FileReader()
{
	m_thread.detach();
}

#pragma warning(push)
#pragma warning(disable:6387)
size_t FileReader::ReadSync(const char* path, char* buffer, size_t bufferSize)
{
	assert(path != nullptr);
	assert(buffer != nullptr);
	assert(bufferSize > 0);
	
	FILE* pFile;
	auto openError = fopen_s(&pFile, path, "r");
	if(openError)
	{
		// TODO: Log error
	}
	
	size_t bytesRead = fread(buffer, 1, bufferSize, pFile);
		
	auto readError = ferror(pFile);
	if(readError)
	{
		bytesRead = 0;
		// TODO: Log error
	}
	
	fclose(pFile);
	
	return bytesRead;
}
#pragma warning(pop)

AsyncRequestId FileReader::ReadAsync(const char*, char*, size_t)
{
	//assert(path != nullptr);
	//assert(buffer != nullptr);
	//assert(bufferSize > 0);

	// TODO: generate async request and submit to a safe queue
	//		 if the queue is empty, wake up read thread
	return 0;
}

void FileReader::WaitAll()
{
	// TODO: Wait for queue to be exhausted
}

void FileReader::WaitSingle(const AsyncRequestId)
{
	// TODO: check if request is in queue
	//		 wait until request is processed
}


void FileReader::ReadThread()
{
	// TODO: if queue is empty, wait until signaled
	//		 otherwise, process until queue is empty and go back to waiting
}