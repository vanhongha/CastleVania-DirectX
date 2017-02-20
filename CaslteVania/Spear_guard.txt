#pragma once
#include "Enemy.h"
class Spear_guard : public Enemy
{
private:
	void InitAnim();
	float timeDelay;
public:
	Spear_guard(int hp, int damage, int point);
	~Spear_guard();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Update(float gameTime);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listObject);
	void CheckColliderWithWall(float gameTime, std::vector<GameObject*> *listObject);
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
};

