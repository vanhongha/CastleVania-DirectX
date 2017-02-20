#include "Graphics.h"
#include "GameMenuState.h"
#include "IntroScene.h"
#include "GamePlayStateOne.h"
#include "GamePlayStateTwo.h"
#include "GamePlayStateThree.h"
#include "Input.h"

class StateManager
{
private:
	Graphics* gDevice;
	GameState* gameState;
	int stateID;
public:
	StateManager();
	~StateManager();
	bool Initialize(HWND hwnd);
	void LoadState(int stateID);
	void NextStateLevel();
	void Render();
	void Update(float gameTime);
};

