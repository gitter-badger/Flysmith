#pragma once
#include "PCH.h"
#include "Timer.h"


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