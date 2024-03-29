#pragma once
#include "PublicDef.h"
#include <vector>


struct Event;

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