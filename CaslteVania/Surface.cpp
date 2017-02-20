#include "Surface.h"



Surface::Surface()
{
	
}


Surface::~Surface()
{
}

void Surface::Render(ViewPort *viewPort)
{
	sprite->Render(viewPort);
}

bool Surface::Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y)
{
	if (!Enemy::Initialize(gDevice, SURFACE_SPRITE, x, y, TAG_CRUSHER))
		return false;

	velocity.x = 0.0f;
	velocity.y = 0.0f;
	return true;
}
