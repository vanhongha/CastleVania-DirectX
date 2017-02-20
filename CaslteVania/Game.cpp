#include "Game.h"
Game::Game()
{
	gDevice = NULL;
	gameTime = NULL;
}


Game::~Game()
{
	SAFE_DELETE(gDevice);
	SAFE_DELETE(viewPort);
	SAFE_DELETE(gameTime);
}

bool Game::Initialize(HWND hWnd, int width, int height)
{
	if ( !InitDirectInput() || !InitKeyboard(hWnd))
		return false;

	Sound::GetInstance()->loadSound(hWnd);

	gameTime = new GameTime();
	if (!gameTime->Initialize())
		return false;

	stateManager = new StateManager();

	//timeStart = GetTickCount();
	if (!stateManager->Initialize(hWnd))
		return false;

	stateManager->LoadState(GAME_MENU_STATE);

	return true;
}

void Game::Run()
{
	//this->TimeHandle();
	
	gameTime->Update();

	Update(gameTime->m_elapsedGameTime);
	Draw();
}

// Update our sprites and other game logics
void Game::Update(float _gameTime)
{
	PollKeyboard();

	stateManager->Update(_gameTime);
}

void Game::Draw()
{
	stateManager->Render();
}

/*void Game::TimeHandle()
{
	timeNow = GetTickCount();
	gameTime = (float)(timeNow - timeStart) / 1000;

	if (gameTime > MAX_TIME_PER_FRAME)
		gameTime = MAX_TIME_PER_FRAME;

	timeStart = timeNow;
}*/