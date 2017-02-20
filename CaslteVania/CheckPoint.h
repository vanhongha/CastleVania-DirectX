#pragma once
#include "GameObject.h"
class CheckPoint :	public GameObject
{
public:
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	CheckPoint();
	~CheckPoint();
};

