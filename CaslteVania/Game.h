#ifndef GAME_H
#define GAME_H

#include "Graphics.h"
#include "Input.h"
#include "Sound.h"
#include "Sprite.h"
#include "Simon.h"
#include "Collider.h"
#include "Zombie.h"
#include "UI.h"
#include "StateManager.h"
#include "GameTime.h"

class Game
{
private:
	Graphics* gDevice;
	ViewPort* viewPort;

	DWORD timeStart;
	DWORD timeNow;
	//float gameTime;
	GameTime* gameTime;
	//void TimeHandle();

	StateManager* stateManager;
public:
	Game();
	~Game();

	// game function
	bool Initialize(HWND hWnd, int width, int height);
	void Run(); // will call update and draw
	void Update(float _gameTime);
	void Draw();

};

#endif // !GAME_H


