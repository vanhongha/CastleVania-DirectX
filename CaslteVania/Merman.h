#pragma once
#include "Enemy.h"
#include "Projectile.h"
class Merman : public Enemy
{
private:
	void InitAnim();
	Projectile *projectile;
	float timeDelay;
public:
	Merman(int hp, int damage, int point);
	~Merman();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listObject);
	Projectile* GetProjectile() { return projectile; }
	void SetJump();
};

