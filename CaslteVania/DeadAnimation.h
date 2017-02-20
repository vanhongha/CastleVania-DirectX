#pragma once
#include "Sprite.h"
#include "Animation.h"

class DeadAnimation
{
private:
	Sprite* sprite;
	Animation *anim;
public:
	DeadAnimation();
	~DeadAnimation();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
	void SetPosition(float x, float y);
	bool CheckDoAllFrame();
};

