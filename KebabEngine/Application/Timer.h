#pragma once
#include "KebabEngine.h"


class KEBAB_API Timer
{
public:
	Timer();

	void Update();
	void Reset();

	// Returns time since creation in seconds.
	float GetRuntime();
	// Returns time delta for the previous frame in seconds.
	float GetElapsed();
	
	U32 GetFrameCount();
	U32 GetFramerate();

private:
	float m_delta;
	U32 m_framerate;
	U32 m_frameCount;

	U64 m_startupTicks;
	U64 m_currentTicks;
	U64 m_oneSecTicks;
	U64 m_lastTicks;
	
	static U64 s_ticksPerSecond;
};