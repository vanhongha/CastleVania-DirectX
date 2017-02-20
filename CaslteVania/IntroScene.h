#pragma once
#include "GameState.h"
#include "TileMap.h"
#include "BatAnimation.h"
#include "Crusher.h"

class IntroScene : public GameState
{
private:
	float timeDelay;
	TileMap *tileMap;

	Sprite *sprite;
	Sprite *helicopter;

	Ground *ground;

	BatAnimation *bat[2];

	void InitAnim();

	LPDIRECT3DDEVICE9 gDevice;
public:
	IntroScene();
	~IntroScene();

	bool Initialize(Graphics *graphics);
	void Render();
	void Update(float gameTime);

	bool GetChangingState();
	void SetChangingState(bool status);
	bool CameraFollowHandle(float gameTime) { return true; }
	void DestroyAll();
};