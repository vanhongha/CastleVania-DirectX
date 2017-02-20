#pragma once
#include "Enemy.h"
#include "Projectile.h"

class BonePillar :
	public Enemy
{
private:
	void InitAnim();
	Projectile *projectile[2];
	float timeDelay;
	bool isProjectileLeft;
public:
	BonePillar(int hp, int damage, int point);
	~BonePillar();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listObject);
	Projectile** GetProjectile() { return projectile; }
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
};

