#include "SceneCheck.h"



bool SceneCheck::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!GameObject::Initialize(gDevice, file, x, y, tag))
		return false;
	return true;
}

SceneCheck::SceneCheck()
{
}


SceneCheck::~SceneCheck()
{
}
