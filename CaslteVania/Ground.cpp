#include "Ground.h"



bool Ground::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!GameObject::Initialize(gDevice, file, x, y, tag))
		return false;

	sprite->Initialize(gDevice, file);
	return true;
}

Ground::Ground(int width, int height)
{
	this->width = width;
	this->height = height;
}


Ground::~Ground()
{
}

void Ground::Render(ViewPort *viewPort)
{
	D3DXVECTOR3 pos = this->position;

	for (int j = 0; j < height / 32; j++)
	{
		pos.y = this->position.y - 32 * j;
		for (int i = 0; i < width / 32; i++)
		{
			pos.x = this->position.x + 32 * i;
			sprite->Render(pos, D3DCOLOR_XRGB(255, 255, 255), viewPort);
		}
	}
}