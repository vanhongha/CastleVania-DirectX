#pragma once
#include "Sprite.h"

class CollideEffect
{
private:
	Sprite *sprite;
	bool isVisible;
	float timeDelay;
public:
	CollideEffect();
	~CollideEffect();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
	void SetPosition(float x, float y);
	void SetVisible(bool _isVisible) { isVisible = _isVisible; }
};

