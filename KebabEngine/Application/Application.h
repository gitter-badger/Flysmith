#pragma once
#include "KebabEngine.h"
#include "Timer.h"
#include <Windows.h>


namespace cuc
{
	class Window;

	class KEBAB_API Application
	{
	public:
		Application(HINSTANCE);
		virtual ~Application();

		virtual U32 Run();

	private:
		Timer   m_timer;
		Window* m_pWindow;
	};
}