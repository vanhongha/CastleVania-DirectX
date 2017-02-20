#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Batman.h"
#include "Medusa.h"

class SubWeapon : public GameObject
{
protected:
	int action;
	D3DXVECTOR3 velocity;
	Animation *anim;
	int useEnergy;
	int damage;
public:
	SubWeapon();
	~SubWeapon();
	virtual void Update(float gameTime);
	virtual bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	virtual void InitAnim() = 0;
	virtual void Render(ViewPort *viewPort);
	virtual void Threw(bool isLeft, float x, float y) = 0;
	virtual void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject);
	virtual bool CheckColliderWith(float gameTime, GameObject *gameObject);
	virtual void CheckColliderWithBoss(float gameTime, GameObject *object);
	void SetVelocity(D3DXVECTOR3 velocity) { this->velocity = velocity; }
	int GetUseEnergy() { return useEnergy; }
	int GetDamage() { return damage; }
};

