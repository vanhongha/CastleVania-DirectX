#pragma once
#include "Enemy.h"

#define HANG	0
#define FLY		1

class BlueBat :
	public Enemy

{
private:
	void InitAnim();
	float delayTime;
public:
	BlueBat(int hp, int damage, int point);
	~BlueBat();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Update(float gameTime);
	void SetAction(int action) { this->action = action; }
	int GetAction() { return action; }
	void SetIsLeft(bool status) { isLeft = status; }
};

