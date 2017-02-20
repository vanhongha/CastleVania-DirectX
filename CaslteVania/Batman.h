#pragma once
#include "Enemy.h"
#include "Zombie.h"
#include <time.h>

#define BATMAN_SLEEP	0
#define BATMAN_FLY		1
#define BATMAN_HURTED	2

#define HURT_TIME	0.2
#define WAIT_TIME	0.75
#define CHASE_TIME	0.1

# define M_PI	3.14159265358979323846

class Batman : public Enemy
{
private:
	void InitAnim();
	float y;

	///////////////////////////
	D3DXVECTOR2 direction;
	D3DXVECTOR2 random;
	D3DXVECTOR2 simonPos;

	float distance;
	RECT* restriction;
	bool atSpot;
	bool aimable;
	bool initialized;
	float waitTime;
	float hurtTime;

	int previousHP; // Check if there is any change in HP (loss HP ~ Get hit)
	float introTime;
	float chaseTime;

public:
	Batman(int hp, int damage, int point);
	~Batman();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
	void Reset();

	////////////////////////////////////
	void Aim(GameObject *object, float gameTime);
	void Sleep();
	void Fly(float gameTime);

	void Restrict(RECT *area);
	void RandomSpot();

	bool isCollided;
	bool isHurted;

	bool IsInitialized() { return initialized; }

	bool Intro(float gameTime);
	void CheckHPChange();
	void CheckHurtTime(float gameTime);
	void FlyToRandomSpot(float gameTime);
};

