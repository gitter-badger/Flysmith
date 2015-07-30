#pragma once
#include "PublicDef.h"
#include <string>


namespace cuc
{
	class KEBAB_API FileSystem
	{
	public:
		FileSystem();
		const wchar_t* GetExePath() const;
		void RemoveLastNameFromPath(std::wstring* path, bool bLeaveTrailingBackslash = true);

	private:
		struct Impl;
		static std::unique_ptr<Impl> s_pImpl;
	};
}