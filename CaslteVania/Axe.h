#pragma once
#include "SubWeapon.h"
class Axe :	public SubWeapon
{
private:
	bool isFiring;		//dang chay
public:
	Axe();
	~Axe();
	void Update(float gameTime);
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void InitAnim();
	void Threw(bool isLeft, float x, float y);
	void Render(ViewPort *viewPort);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject);
	void CheckColliderWithBoss(float gameTime, GameObject *object);
	void SetVelocity(D3DXVECTOR3 velocity) { this->velocity = velocity; }
};

