#pragma once
#include "GameObject.h"
class Projectile :
	public GameObject
{
	D3DXVECTOR3 velocity;
	bool isLeft;
	Animation *anim;
public:
	Projectile();
	~Projectile();
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
	void SetIsLeft(bool _isLeft);
	void SetVelocity(float vx, float vy);
};

