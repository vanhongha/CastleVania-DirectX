#pragma once
#include "SubWeapon.h"
#include "Animation.h"
#include "Simon.h"

class Cross : public SubWeapon
{
private:
	bool isCollidedWithEnemy;
	bool isTurnBack;
	bool left;
	D3DXVECTOR3 simonPos;
	Animation *anim;
public:
	Cross();
	~Cross();
	void Update(float gameTime);
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void InitAnim();
	void Threw(bool isLeft, float x, float y);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject);
	bool CheckColliderWith(float gameTime, GameObject *gameObject);
	void CheckTurnBack(float gameTime, ViewPort *viewPort);
	void Render(ViewPort *viewPort);
	bool IsTurnBack() { return isTurnBack; }
	void SetDefaultProperties();
};

