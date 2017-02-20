#ifndef GAMETIME_H
#define GAMETIME_H

#include <Windows.h>
#include "Input.h"

class GameTime
{
public:
	float m_elapsedGameTime;
	float m_totalGameTime;

	bool Initialize();
	bool Initialize(int fps);

	void Update();
	void SetFPS(int fps);

	GameTime(void);
	~GameTime(void);

private:
	LONGLONG	m_start;
	float		m_frequencySeconds; // Seconds per count
	float		m_fps;				// frames per second
};

#endif
