#pragma once
#include "GameState.h"
#include "Sprite.h"
#include "UI.h"
#include "Input.h"
#include "TitleAnimation.h"

class GameMenuState : public GameState
{
private:
	Sprite* sprite;
	TitleAnimation *animation;
public:
	GameMenuState();
	~GameMenuState();
	bool Initialize(Graphics *graphics);
	void Render();
	void Update(float gameTime);
	void DestroyAll();
	bool GetChangingState() { return GameState::GetChangingState(); }
	void SetChangingState(bool status) { GameState::SetChangingState(status); }
	bool CameraFollowHandle(float gameTime) { return true; }
};

