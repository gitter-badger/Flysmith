#include "PCH.h"
#include "Logger.h"
#include "FileSystem.h"


Logger::Logger()
{
}

Logger::~Logger()
{
	Close();
}

void Logger::Init()
{
	FileSystem fs;
	std::wstring path = std::wstring(fs.GetExePath()) + L"Log" + L".txt";
	m_logStream.open(path);
	Write("Log file opened.");
}

void Logger::Close()
{
	Write("Log file closed.");
	m_logStream << m_buffer;
	m_logStream.close();
}

void Logger::Write(const std::string& msg, bool writeSeparator)
{
#ifdef _DEBUG
	OutputDebugStringA(msg.c_str());
	OutputDebugStringA("\n");
#endif

	m_buffer += msg + "\n";
	if (writeSeparator)
		WriteSeparator();
}

void Logger::Write(F32 msg, bool writeSeparator)
{
	Write(std::to_string(msg), writeSeparator);
}

void Logger::Write(U32 msg, bool writeSeparator)
{
	Write(std::to_string(msg), writeSeparator);
}

void Logger::Write(I32 msg, bool writeSeparator)
{
	Write(std::to_string(msg), writeSeparator);
}

void Logger::WriteSeparator()
{
	m_buffer += "----------------------------------------------------\n";
}