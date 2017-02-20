#pragma once
#include "GameObject.h"
class Stair :
	public GameObject
{
public:
	Stair();
	~Stair();
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
};

