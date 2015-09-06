#pragma once
#include "PublicDef.h"
#include <string>
#include <fstream>


#define LogWriteScoped(msg) g_logger.Write(std::string(__FILE__) + ": " + std::string(__func__) + ", " + std::to_string(__LINE__) + ": " + msg);

class KEBAB_API Logger
{
public:
	Logger();
	~Logger();

	void Init();
	void Close();

	void Write(const std::string&, bool writeSeparator = true);
	void Write(F32, bool writeSeparator = true);
	void Write(U32, bool writeSeparator = true);
	void Write(I32, bool writeSeparator = true);
	void WriteSeparator();

private:
	std::string	  m_buffer;
	std::ofstream m_logStream;
};

extern KEBAB_API Logger g_logger;