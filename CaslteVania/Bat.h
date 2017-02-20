#pragma once
#include "Enemy.h"
#include <math.h>

class Bat : public Enemy
{
private:
	void InitAnim();
	float y;
public:
	Bat(int hp, int damage, int point);
	~Bat();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Update(float gameTime);
	void SetY(float _y) { y = _y; }
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
};

