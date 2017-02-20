#pragma once
#include "Enemy.h"

class Snake : public Enemy
{
protected:
	void InitAnimation();
public:
	Snake(int hp, int damage, int point);
	~Snake();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Update(float gameTime);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listObject);
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
};

