#pragma once
#include "KebabEngine.h"
#include <vector>


namespace cuc
{
	class Event;

	class KEBAB_API EventListener
	{
	public:
		virtual ~EventListener();
		const std::vector<U32>& GetEvents() const;

		virtual void HandleEvent(const Event&) = 0;

	protected:
		void RegisterForEvent(U32 hashedEventType);

	private:
		std::vector<U32> m_events;
	};
}