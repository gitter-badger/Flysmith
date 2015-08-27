#include "PCH.h"
#include "Timer.h"


U64 Timer::s_ticksPerSecond = 0;

Timer::Timer()
	: m_delta(0.0f)
	, m_framerate(0)
	, m_frameCount(0)
	, m_startupTicks(0)
	, m_oneSecTicks(0)
	, m_lastTicks(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&s_ticksPerSecond);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTicks);
	
	m_startupTicks = m_currentTicks;
	m_oneSecTicks  = m_currentTicks;
}

void Timer::Update()
{
	m_lastTicks = m_currentTicks;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTicks);

	m_delta = static_cast<float>(m_currentTicks - m_lastTicks) / s_ticksPerSecond;

	if (static_cast<float>((m_currentTicks - m_oneSecTicks)) / s_ticksPerSecond < 1.0f)
		m_frameCount++;
	else
	{
		m_framerate = m_frameCount;

		m_frameCount = 0;
		m_oneSecTicks = m_currentTicks;
	}
}

void Timer::Reset()
{
	m_frameCount = 0;
	m_delta = 0;
	m_framerate = 0;
}

float Timer::GetRuntime()
{
	return static_cast<float>(m_currentTicks - m_startupTicks) / s_ticksPerSecond;
}

float Timer::GetElapsed()
{
	return m_delta;
}

U32 Timer::GetFrameCount()
{
	return m_frameCount;
}

U32 Timer::GetFramerate()
{
	return m_framerate;
}