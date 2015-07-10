#pragma once
#include "KebabEngine.h"
#include <thread>


namespace cuc
{
	using AsyncRequestId = U32;
	
	class AsyncFileRequest
	{
	public:
		enum class Status
		{
			NOT_STARTED,
			STARTED,
			FINISHED
		};
		
		const Status GetStatus() const { return m_status; }
		
	private:
		Status m_status;
			
	};
	
	class KEBAB_API FileReader final
	{
	public:
		FileReader();
		~FileReader();
	
		// Returns number of bytes read
		size_t ReadSync(const char* path, char* buffer, size_t bufferSize);
		
		// 
		AsyncRequestId ReadAsync(const char* path, char* buffer, size_t bufferSize);
		
		// Blocks until all async requests are finished
		void WaitAll();
		
		// Blocks until a single request is finished
		void WaitSingle(const AsyncRequestId);
		
	private:
		void ReadThread();
		std::thread m_thread;
	};
}