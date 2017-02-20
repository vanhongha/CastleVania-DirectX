#pragma once
#include "Enemy.h"
class Surface :
	public Enemy
{
public:
	Surface();
	~Surface();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y);
	void Render(ViewPort *viewPort);
};

