#pragma once
#include "Animation.h"

class BatAnimation
{
private:
	Sprite* sprite;
	Animation *anim;
public:
	BatAnimation();
	~BatAnimation();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y);
	void Render(ViewPort *viewPort);
	void Update(float gameTime, float vx, float vy);
	void SetPosition(float x, float y);
};

