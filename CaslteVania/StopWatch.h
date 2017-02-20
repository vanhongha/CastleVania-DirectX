#pragma once
#include "SubWeapon.h"

class StopWatch : public SubWeapon
{
public:
	StopWatch();
	~StopWatch();
	void Update(float gameTime);
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void InitAnim();
	void Threw(bool isLeft, float x, float y);
	void Render(ViewPort *viewPort);
};

