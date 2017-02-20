#pragma once
#include "Enemy.h"
class MedusaHead : public Enemy
{
private:
	void InitAnim();
	float y;
public:
	MedusaHead(int hp, int damage, int point);
	~MedusaHead();
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Update(float gameTime);
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
	void SetY(float _y) { y = _y; }
};

