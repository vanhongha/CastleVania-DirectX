#pragma once
#include "SubWeapon.h"
class Knife :	public SubWeapon
{
private:
	D3DXVECTOR3 simonPos;
	Animation *anim;
	bool isLeft;
public:
	Knife();
	~Knife();
	void Update(float gameTime);
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void InitAnim();
	void Threw(bool isLeft, float x, float y);
	void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject);
	bool CheckColliderWith(float gameTime, GameObject *gameObject);
	void Render(ViewPort *viewPort);
};

