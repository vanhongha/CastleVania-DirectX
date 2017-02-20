#pragma once
#include "Enemy.h"
#include "Animation.h"
#include "DeadAnimation.h"
#include "Batman.h"
#include "Medusa.h"

#define NORMAL_WHIP		0
#define VAMPIRE_KILLER	1
#define CHAIN_WHIP		2
#define MORNING_STAR	3

class Whip : public GameObject
{
private:
	Animation *anim;
	int type;
	bool isLeft;
	bool check; //kiem tra xem roi da xet va cham chua
public:
	Whip();
	~Whip();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);

	void SetPosition(D3DXVECTOR3 _position, bool isStanding);

	void SetIsLeft(bool _isLeft);

	void SetType(int _type);

	D3DXVECTOR3 GetVelocity() { return D3DXVECTOR3(0, 0, 0); }

	void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject);
	void CheckColliderWith(float gameTime, GameObject *object);

	int GetCurrentFrameOfWhip() { return anim->GetCurrentFrame(); }

	int GetDamage() { return type == VAMPIRE_KILLER ? 3 : 4; }
};

