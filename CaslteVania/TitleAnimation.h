#pragma once
#include "Animation.h"

class TitleAnimation : public Animation
{
	int action;
	Sprite* sprite;
	Animation *anim;
public:
	TitleAnimation();
	~TitleAnimation();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
};

