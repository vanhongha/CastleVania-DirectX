#include "GameMenuState.h"

GameMenuState::GameMenuState()
{
}


GameMenuState::~GameMenuState()
{
}

bool GameMenuState::Initialize(Graphics *graphics)
{
	viewPort = new ViewPort(0, GAME_HEIGHT);
	sprite = new Sprite(0.0f, 480.0f);
	animation = new TitleAnimation();
	changeState = false;

	if (!sprite->Initialize(graphics->GetDevice(), TITLE_SCREEN))
		return false;

	if (!animation->Initialize(graphics->GetDevice(), TITLE_ANIMATION, 446, 255))
		return false;
	return true;
}


void GameMenuState::Render()
{
	//text->Draw();

	sprite->Render(viewPort);
	animation->Render(viewPort);
}

void GameMenuState::Update(float gameTime)
{
	// De update cai animation con doi bay bay sau
	if (IsKeyPress(DIK_RETURN))
		SetChangingState(true);
	animation->Update(gameTime);
}

void GameMenuState::DestroyAll(){}
