#pragma once
#include "Enemy.h"
class Zombie : public Enemy
{
private:
	void InitAnimation();
public:
	Zombie(int hp, int damage, int point);
	~Zombie();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Update(float gameTime);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listObject);
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
	void CheckColliderWithWall(float gameTime, std::vector<GameObject*>* listGameObject);
};
