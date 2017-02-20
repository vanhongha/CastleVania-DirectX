#include "GameTime.h"


GameTime::GameTime(void)
{
}


GameTime::~GameTime(void)
{
}


bool GameTime::Initialize()
{
	LARGE_INTEGER i;

	// Get frequency from the counter. The frequency cannot change while the system is running, so we only need to do this once
	if (!QueryPerformanceFrequency(&i))
		return FALSE;

	// Gives us a number of ticks per second
	m_frequencySeconds = (float)(i.QuadPart);

	// Get the current value of the counter
	QueryPerformanceCounter(&i);
	m_start = i.QuadPart;
	m_totalGameTime = 0;
	m_elapsedGameTime = 0;
	
	return TRUE;
}

void GameTime::Update()
{
		LARGE_INTEGER i;

		QueryPerformanceCounter(&i);
		m_elapsedGameTime = (float)(i.QuadPart - m_start) / m_frequencySeconds;
		m_totalGameTime += m_elapsedGameTime;
		m_start = i.QuadPart;
}
