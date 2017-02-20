#include "IntroScene.h"

void IntroScene::InitAnim()
{

}

IntroScene::IntroScene()
{

}

IntroScene::~IntroScene()
{
}

bool IntroScene::Initialize(Graphics *graphics)
{
	this->gDevice = graphics->GetDevice();

	timeDelay = 0;

	viewPort = new ViewPort(0, GAME_HEIGHT);

	sprite = new Sprite();
	if (!sprite->Initialize(gDevice, INTRO_SCENE_BG))
		return false;

	helicopter = new Sprite();
	if (!helicopter->Initialize(gDevice, "Resource\\sprites\\helicopter.png"))
		return false;
	helicopter->SetPosition(552, 350);

	ground = new Ground(640, 32);
	ground->Initialize(gDevice, "Resource\\sprites\\Ground\\NULL.png", 0, 16, TAG_GROUND);
	ground->SetBox(0, 16 + 8, 640, 30, 0, 0);

	tileMap = new TileMap(640, 448, sprite, 64, 64);
	tileMap->LoadListTileFromFile(INTRO_SCENE_MATRIX_BG);

	changeState = false;

	Sound::GetInstance()->PlayLoop(GAME_START_PROLOGUE);

	for (int i = 0;i < 2;i++)
		bat[i] = new BatAnimation();

	bat[0]->Initialize(gDevice, "Resource\\sprites\\bat.png", 410, 400);
	bat[1]->Initialize(gDevice, "Resource\\sprites\\bat.png", 57, 200);

	simon = new Simon(8, 0, 0);
	if (!simon->Initialize(gDevice, SIMON_SPRITE, 610, 91, TAG_SIMON))
		return false;
	simon->SetIsLeft(true);

	return true;
}

void IntroScene::Render()
{
	tileMap->Draw(viewPort);
	ground->Render(viewPort);
	
	helicopter->Render(viewPort);

	bat[0]->Render(viewPort);
	bat[1]->Render(viewPort);

	simon->Render(viewPort);
}

void IntroScene::Update(float gameTime)
{
	bat[0]->Update(gameTime, -13, -3);
	bat[1]->Update(gameTime, 30, 17);
	helicopter->SetPosition(helicopter->GetPosition().x - 13 * gameTime,
		helicopter->GetPosition().y);

	if (simon->GetPosition().x > viewPort->GetCameraPosition().x + GAME_WIDTH / 2 - 35)
	{
		simon->SetPosition(simon->GetPosition().x - 57 * gameTime, simon->GetPosition().y);
		simon->SetAction(MOVE);
	}
	else
	{
		simon->SetPosition(simon->GetPosition().x, simon->GetPosition().y);
		simon->SetAction(STAND_BACK);
		timeDelay += gameTime;
		if(timeDelay >= 1.5)
			SetChangingState(true);
	}

	simon->Update(gameTime);	
	simon->CheckCollideWithGround(gameTime, ground);

	if(IsKeyPress(DIK_M))
		SetChangingState(true);
}

bool IntroScene::GetChangingState()
{
	return GameState::GetChangingState();
}

void IntroScene::SetChangingState(bool status)
{
	GameState::SetChangingState(status);
}

void IntroScene::DestroyAll()
{	//delete(simon);
	delete(tileMap);
	delete(listObject);
	delete(viewPort);
}

