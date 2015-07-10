#pragma once
#include "KebabEngine.h"
#include "Timer.h"
#include <Windows.h>
#include <memory>


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
		Timer m_timer;
		std::shared_ptr<Window> m_pWindow;
	};
}