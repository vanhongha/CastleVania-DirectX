#pragma once
#include "SubWeapon.h"
#include "Enemy.h"
class HolyWater : public SubWeapon
{
private: 
	float timeDelay;
	bool isFiring;		//dang chay
public:
	HolyWater();
	~HolyWater();
	void Update(float gameTime);	
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void InitAnim();
	void Threw(bool isLeft, float x, float y);
	void Render(ViewPort *viewPort);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject);
	void SetVelocity(D3DXVECTOR3 velocity) { this->velocity = velocity; }
};

