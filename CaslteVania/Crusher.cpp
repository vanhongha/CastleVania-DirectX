#include "Crusher.h"



Crusher::Crusher()
{
}


Crusher::~Crusher()
{
}

void Crusher::Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y)
{
	height = 130;
	position.x = x;
	position.y = y;

	GameObject::Initialize(gDevice, NULL_SPRITE, x, y, TAG_CRUSHER);

	surface = new GameObject();
	surface->Initialize(gDevice, SURFACE_SPRITE, x, y, TAG_CRUSHER);

	surfaceOnTop = false;
	surfaceOnBottom = false;

	for (int i = 0;i < 20;i++)
	{
		enableRope[i] = false;
		rope[i] = new GameObject();
		rope[i]->Initialize(gDevice, ROPE_SPRITE, x + 16 + 8, y - i * 8, TAG_CRUSHER);
	}
}

void Crusher::Render(ViewPort * viewport)
{
	for (int i = 0;i < 20;i++)
		if (enableRope[i])
			rope[i]->Render(viewport);
	surface->Render(viewport);
}

void Crusher::Update(float gameTime)
{
	SurfaceUpdate(gameTime);
	RopeUpdate();
}

void Crusher::SurfaceUpdate(float gameTime)
{
	float tempVy;
	if (surface->GetPosition().y >= this->position.y - 16)
	{
		surfaceOnTop = true;
		surfaceOnBottom = false;
	}
	else if (surface->GetPosition().y - 16 <= this->position.y - height)
	{
		surfaceOnBottom = true;
		surfaceOnTop = false;
	}

	if (surfaceOnTop)
	{
		Setposition(surface, D3DXVECTOR3(0, -120, 0), gameTime);
		tempVy = -120;
	}
	else if (surfaceOnBottom)
	{
		Setposition(surface, D3DXVECTOR3(0, 70, 0), gameTime);
		tempVy = 70;
	}

	SetBox(surface->GetPosition().x,
		surface->GetPosition().y,
		64,
		16,
		0,
		tempVy);
}

void Crusher::RopeUpdate()
{
	float offsetHeight = this->position.y - surface->GetPosition().y;

	for (int i = 0;i < (int)offsetHeight / 8;i++)
	{
		enableRope[i] = true;
		enableRope[i + 1] = true;
	}

	for (int i = (int)offsetHeight / 8; i < 20;i++)
		enableRope[i + 1] = false;
}

void Crusher::Setposition(GameObject * object, D3DXVECTOR3 v, float gameTime)
{
	object->SetPosition(object->GetPosition().x + v.x * gameTime,
		object->GetPosition().y + v.y*gameTime);
}
