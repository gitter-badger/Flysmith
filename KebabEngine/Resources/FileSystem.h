#pragma once
#include "PCH.h"


namespace cuc
{
	class KEBAB_API FileSystem
	{
	public:
		FileSystem();
		const char* GetExePath();
		const char* GetAssetsPath();
		const char* GetShadersPath();
		const char* GetFontsPath();

	private:
		struct Impl;
		static std::unique_ptr<Impl> s_pImpl;
	};
}